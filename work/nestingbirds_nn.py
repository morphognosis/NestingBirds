# For conditions of distribution and use, see copyright notice in LICENSE.txt

# Nesting birds NN.
# results written to nestingbirds_nn_results.txt
from numpy import array, argmax
from numpy import loadtxt
from keras.models import Sequential
from keras.layers import Dense
import sys, getopt

# parameters
n_neurons = 128
n_epochs = 500

# results file name
results_filename = 'nestingbirds_nn_results.json'

# get options
try:
  opts, args = getopt.getopt(sys.argv[1:],"hn:e:",["neurons=","epochs="])
except getopt.GetoptError:
  print('nestingbirds_nn.py [-n <hidden_neurons>] [-e <epochs>]')
  sys.exit(1)
for opt, arg in opts:
  if opt == '-h':
     print('nestingbirds_nn.py [-n <hidden_neurons>] [-e <epochs>]')
     sys.exit(0)
  if opt in ("-n", "--neurons"):
     n_neurons = int(arg)
  elif opt in ("-e", "--epochs"):
     n_epochs = int(arg)
  else:
     print('nestingbirds_nn.py [-n <hidden_neurons>] [-e <epochs>]')
     sys.exit(1)

# import dataset
from nestingbirds_nn_dataset import X_train_shape, y_train_shape, X_train, y_train, X_test_shape, y_test_shape, X_test, y_test
if X_train_shape[0] == 0:
    print('Empty train dataset')
    sys.exit(1)
if X_test_shape[0] == 0:
    print('Empty test dataset')
    sys.exit(1)
seq = array(X_train)
print(seq)
print(len(seq))
print(X_train_shape)
X = seq.reshape(X_train_shape[0], X_train_shape[1])
seq = array(y_train)
y = seq.reshape(y_train_shape[0], y_train_shape[1])

# create NN
model = Sequential()
model.add(Dense(n_neurons, input_dim=X_train_shape[1], activation='relu'))
model.add(Dense(n_neurons, activation='relu'))
model.add(Dense(y_train_shape[1], activation='sigmoid'))
model.compile(loss='binary_crossentropy', optimizer='adam', metrics=['accuracy'])

# train
model.fit(X, y, epochs=n_epochs, batch_size=10)
_, accuracy = model.evaluate(X, y)
print('Accuracy: %.2f' % (accuracy*100))
#model.save("model.dat")

# validate
seq = array(X_train)
X = seq.reshape(X_train_shape[0], X_train_shape[1])
seq = array(y_train)
y = seq.reshape(y_train_shape[0], y_train_shape[1])
predictions = model.predict(X, batch_size=X_train_shape[0], verbose=2)
trainErrors = 0
trainTotal = 0
for response in range(y_train_shape[0]):
    if not all([ v == 0 for v in y[response]]):
        p = argmax(predictions[response])
        t = argmax(y[response])
        if p != t:
            trainErrors += 1
        trainTotal += 1

# predict
seq = array(X_test)
X = seq.reshape(X_test_shape[0], X_test_shape[1])
seq = array(y_test)
y = seq.reshape(y_test_shape[0], y_test_shape[1])
testErrors = 0
testTotal = 0
if X_test_shape[0] > 0:
    predictions = model.predict(X, batch_size=X_test_shape[0], verbose=2)
    for response in range(y_test_shape[0]):
        if not all([ v == 0 for v in y[response]]):
            p = argmax(predictions[response])
            t = argmax(y[response])
            if p != t:
                testErrors += 1
            testTotal += 1

# Print results.
print("Train prediction errors/total = ", trainErrors, "/", trainTotal, sep='', end='')
trainErrorPct=0
if trainTotal > 0:
    trainErrorPct = (float(trainErrors) / float(trainTotal)) * 100.0
    print(" (", str(round(trainErrorPct, 2)), "%)", sep='', end='')
print('')
print("Test prediction errors/total = ", testErrors, "/", testTotal, sep='', end='')
testErrorPct=0
if testTotal > 0:
    testErrorPct = (float(testErrors) / float(testTotal)) * 100.0
    print(" (", str(round(testErrorPct, 2)), "%)", sep='', end='')
print('')

# Write results to file.
with open(results_filename, 'w') as f:
    f.write('{')
    f.write('\"train_prediction_errors\":\"'+str(trainErrors)+'\",')
    f.write('\"train_total_predictions\":\"'+str(trainTotal)+'\",')
    f.write('\"train_error_pct\":\"'+str(round(trainErrorPct, 2))+'\",')
    f.write('\"test_prediction_errors\":\"'+str(testErrors)+'\",')
    f.write('\"test_total_predictions\":\"'+str(testTotal)+'\",')
    f.write('\"test_error_pct\":\"'+str(round(testErrorPct, 2))+'\"')
    f.write('}\n')


sys.exit(0)
