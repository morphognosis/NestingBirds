# For conditions of distribution and use, see copyright notice in Mandala.java
# World grammar learning TCN.

# Default parameters.
dataset_filename = 'world_grammar_dataset.csv'
sequence_length = 60
num_predictions = 7
n_kernels = 2
n_epochs = 50
results_filename = 'world_grammar_tcn_results.json'
plot = False
verbose = False

# Get options.
import getopt
import sys
usage = 'usage: python world_grammar_tcn.py [--dataset_filename <file name>] [--sequence_length <length>] [--num_predictions <number of predictions>] [--kernel_size <kernel size>] [--epochs <number of epochs>] [--results_filename <filename>] [--plot] [--verbose]'
try:
  opts, args = getopt.getopt(sys.argv[1:],"h",["help","dataset_filename=","sequence_length=","num_predictions=","kernel_size=","epochs=","results_filename=","plot","verbose"])
except getopt.GetoptError:
  print(usage)
  sys.exit(1)
for opt, arg in opts:
  if opt == '-h' or opt == '--help':
     print(usage)
     sys.exit(0)
  if opt == "--dataset_filename":
     dataset_filename = arg
  elif opt == "--sequence_length":
     sequence_length = int(arg)
  elif opt == "--num_predictions":
     num_predictions = int(arg)
  elif opt == "--kernel_size":
     n_kernels = int(arg)
  elif opt == "--epochs":
     n_epochs = int(arg)
  elif opt == "--results_filename":
     results_filename = arg
  elif opt == "--plot":
     plot = True
  elif opt == "--verbose":
     verbose = True
  else:
     print(usage)
     sys.exit(1)

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from tensorflow.keras import Sequential
from tensorflow.keras.layers import Dense
from tcn import TCN

# Load dataset.
import pandas as pd
import numpy as np

df=None
try:
  df=pd.read_csv(dataset_filename)
except FileNotFoundError:
  print('Cannot open', dataset_filename)
  sys.exit(1)
df['date']=pd.to_datetime(df.date,format="%Y-%m-%d")
df.index=df['date']

data=df.sort_index(ascending=True,axis=0)
dataset=pd.DataFrame(index=range(0,len(df)),columns=['date','close'])
for i in range(0,len(data)):
    dataset['date'][i]=data['date'][i]
    dataset['close'][i]=data['close'][i]
dataset.index=dataset.date
dataset.drop('date',axis=1,inplace=True)
dataset_size = len(dataset)

# Determine train and test set sizes.
if dataset_size < sequence_length:
    print('dataset size (', dataset_size, ') cannot be less than sequence_length (', sequence_length, ')', sep='')
    sys.exit(1)
if not train_all and dataset_size < num_predictions + sequence_length:
    print('dataset size (', dataset_size, ') cannot be less than num_predictions + sequence_length (', (num_predictions + sequence_length), ')', sep='')
    sys.exit(1)
test_dataset_size = num_predictions
if test_dataset_size == 0:
    print('error: test dataset size = 0')
    sys.exit(1)
if train_all:
    train_dataset_size = dataset_size
else:
    train_dataset_size = dataset_size - test_dataset_size + 1
if train_dataset_size < sequence_length:
    print('error: train dataset size < sequence_length')
    sys.exit(1)

# Create training data.
values=dataset.values.copy()
validate_data=values[dataset_size - test_dataset_size + 1:,:].copy()
base_stock_value=values[dataset_size - test_dataset_size,0]
current_stock_value=values[dataset_size - 1,0]
from sklearn.preprocessing import MinMaxScaler
scaler=MinMaxScaler(feature_range=(0,1))
values=scaler.fit_transform(values)
train_data=values[0:train_dataset_size,:].copy()
x_train_data,y_train_data=[],[]
for i in range(sequence_length,len(train_data)):
    x_train_data.append(train_data[i-sequence_length:i,0])
    y_train_data.append(train_data[i,0])
x_train_data,y_train_data=np.array(x_train_data),np.array(y_train_data)
x_train_data=np.reshape(x_train_data,(x_train_data.shape[0],x_train_data.shape[1],1))

# Create TCN model.
tcn_model = Sequential([
    TCN(input_shape=(sequence_length, 1),
        kernel_size=n_kernels,
        use_skip_connections=False,
        use_batch_norm=False,
        use_weight_norm=False,
        use_layer_norm=False
        ),
    Dense(1, activation='linear')
])
tcn_model.compile('adam', 'mae')
#tcn_model.summary()

# Train model.
tcn_model.fit(x_train_data,y_train_data, epochs=n_epochs, verbose=0)

# Create test data.
test_dataset=dataset[dataset_size-len(validate_data)-sequence_length:].values.copy()
test_data=test_dataset.reshape(-1,1)
test_data=scaler.transform(test_dataset)
x_test_data=[]
for i in range(sequence_length,test_data.shape[0] + 1):
    x_test_data.append(test_data[i-sequence_length:i,0])
x_test_data=np.array(x_test_data)
x_test_data=np.reshape(x_test_data,(x_test_data.shape[0],x_test_data.shape[1],1))

# Predict test data.
prediction_data=tcn_model.predict(x_test_data)
prediction_data=scaler.inverse_transform(prediction_data)

# Initialize optimal and actual stock and cash values.
current_stock_value=base_stock_value
optimal_stock_value=current_stock_value
actual_stock_value=current_stock_value
optimal_cash_value=0.0
actual_cash_value=0.0

# Calculate error and gains/losses.
error = 0.0
validate_range = len(validate_data)
for i in range(validate_range):
    error += abs(validate_data[i,0] - prediction_data[i,0])

    if verbose:
      print('current stock value=', round(current_stock_value, 5), sep='');
      print('validation value=', round(validate_data[i,0], 5), sep='')
      print('prediction value=', round(prediction_data[i,0], 5), sep='')
      print('optimal stock value=', round(optimal_stock_value, 5), sep='')
      print('optimal cash value=', round(optimal_cash_value, 5), sep='')
      print('actual stock value=', round(actual_stock_value, 5), sep='')
      print('actual cash value=', round(actual_cash_value, 5), sep='')

    # Ideal buy/sell.
    if validate_data[i,0] > current_stock_value:
        if optimal_stock_value == 0.0:
            optimal_cash_value -= current_stock_value
            if verbose:
               print('optimal buy: ', round(validate_data[i,0] - current_stock_value, 5), sep='')
        optimal_stock_value = validate_data[i,0]
    elif validate_data[i,0] < current_stock_value:
        if optimal_stock_value > 0.0:
            optimal_cash_value += optimal_stock_value
            optimal_stock_value = 0.0
            if verbose:
               print('optimal sell: ', round(current_stock_value - validate_data[i,0], 5), sep='')

    # Buy/sell based on prediction.
    if prediction_data[i,0] > current_stock_value:
        if actual_stock_value == 0.0:
            actual_cash_value -= current_stock_value
            if verbose:
               print('actual buy: ', round(validate_data[i,0] - current_stock_value, 5), sep='')
        actual_stock_value = validate_data[i,0]
    elif prediction_data[i,0] < current_stock_value:
        if actual_stock_value > 0.0:
            actual_cash_value += actual_stock_value
            actual_stock_value = 0.0
            if verbose:
               print('actual sell: ', round(current_stock_value - validate_data[i,0], 5), sep='')
    else:
        if actual_stock_value > 0.0:
            actual_stock_value = validate_data[i,0]
            if verbose:
               print('actual hold: ', round(validate_data[i,0] - currrent_stock_value, 5), sep='')

    current_stock_value = validate_data[i,0]

error /= len(validate_data)
if verbose:
   print('mean prediction error=', round(error, 5), sep='')
actual_gain=(actual_stock_value + actual_cash_value) - base_stock_value
if verbose:
   print('actual gain=', round(actual_gain, 5), sep='')
optimal_gain=(optimal_stock_value + optimal_cash_value) - base_stock_value
if verbose:
   print('optimal gain=', round(optimal_gain, 5), sep='')
   print('current value=', round(current_stock_value, 5), sep='')
   print('predicted value=', round(prediction_data[validate_range,0], 5), sep='')

# Write results to file.
with open(results_filename, 'w') as f:
    f.write('{')
    f.write('\"mean_prediction_error\":\"'+str(error)+'\",')
    f.write('\"actual_gain\":\"'+str(actual_gain)+'\",')
    f.write('\"optimal_gain\":\"'+str(optimal_gain)+'\",')
    f.write('\"current_value\":\"'+str(current_stock_value)+'\",')
    f.write('\"predicted_value\":\"'+str(prediction_data[validate_range,0])+'\"')
    f.write('}\n')

# Plot?
if plot:
    import matplotlib.pyplot as plt
    train_plot_data=dataset[:train_dataset_size]
    if train_all:
        comparison_plot_data=dataset[dataset_size - test_dataset_size + 1:dataset_size]
    else:
        comparison_plot_data=dataset[train_dataset_size:dataset_size]
    comparison_plot_data['predictions']=prediction_data[:validate_range]
    plt.figure(figsize=(16,8))
    plt.plot(train_plot_data['close'], label='train')
    if train_all == False:
        plt.plot(comparison_plot_data[['close']], label='validation')
    plt.plot(comparison_plot_data[['predictions']], label='prediction')
    plt.legend(loc='upper left')
    plt.title(symbol)
    plt.xlabel('year')
    plt.ylabel('closing price')
    plt.show()

sys.exit(0)
