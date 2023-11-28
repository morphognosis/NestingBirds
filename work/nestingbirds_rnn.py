# For conditions of distribution and use, see copyright notice in LICENSE.txt

# Nesting birds RNN.
# results written to nestingbirds_rnn_results.txt
from numpy import array, argmax
from keras.models import Sequential
from keras.layers import Dense
from keras.layers import TimeDistributed
from keras.layers import LSTM
import sys, getopt

# define LSTM configuration
n_neurons = 128
n_epochs = 500

# results file name
results_filename = 'nestingbirds_rnn_results.json'

# get options
try:
  opts, args = getopt.getopt(sys.argv[1:],"hn:e:",["neurons=","epochs="])
except getopt.GetoptError:
  print('nestingbirds_rnn.py [-n <neurons>] [-e <epochs>]')
  sys.exit(1)
for opt, arg in opts:
  if opt == '-h':
     print('nestingbirds_rnn.py [-n <neurons>] [-e <epochs>]')
     sys.exit(0)
  if opt in ("-n", "--neurons"):
     n_neurons = int(arg)
  elif opt in ("-e", "--epochs"):
     n_epochs = int(arg)

# prepare data
from nestingbirds_rnn_dataset import X_train_shape, X_train_seq, y_train_shape, y_train_seq
seq = array(X_train_seq)
X = seq.reshape(X_train_shape[0], X_train_shape[1], X_train_shape[2])
seq = array(y_train_seq)
y = seq.reshape(y_train_shape[0], y_train_shape[1], y_train_shape[2])

# create LSTM
model = Sequential()
model.add(LSTM(n_neurons, input_shape=(X_train_shape[1], X_train_shape[2]), return_sequences=True))
model.add(TimeDistributed(Dense(y_train_shape[2])))
model.compile(loss='mean_squared_error', optimizer='adam')
#print(model.summary())

# train
model.fit(X, y, epochs=n_epochs, batch_size=X_train_shape[0], verbose=2)

# validate training
seq = array(X_train_seq)
X = seq.reshape(X_train_shape[0], X_train_shape[1], X_train_shape[2])
seq = array(y_train_seq)
y = seq.reshape(y_train_shape[0], y_train_shape[1], y_train_shape[2])
predictions = model.predict(X, batch_size=X_train_shape[0], verbose=2)
trainOK = 0
trainErrors = 0
trainTotal = 0
for path in range(X_train_shape[0]):
    p = []
    for step in range(X_train_shape[1]):
        if not all([ v == 0 for v in y[path][step]]):
            r = argmax(predictions[path][step])
            p.append(r)
    t = []
    for step in range(X_train_shape[1]):
        if not all([ v == 0 for v in y[path][step]]):
            r = argmax(y[path][step])
            t.append(r)
    if p == t:
        trainOK += 1
    else:
        errs = 0
        for i in range(len(p)):
            if p[i] != t[i]:
                errs += 1
        trainErrors += errs
    trainTotal += len(p)

# predict
from nestingbirds_rnn_dataset import X_test_shape, X_test_seq, y_test_shape, y_test_seq
seq = array(X_test_seq)
X = seq.reshape(X_test_shape[0], X_test_shape[1], X_test_shape[2])
seq = array(y_test_seq)
y = seq.reshape(y_test_shape[0], y_test_shape[1], y_test_shape[2])
testOK = 0
testErrors = 0
testTotal = 0
if X_test_shape[0] > 0:
    predictions = model.predict(X, batch_size=X_test_shape[0], verbose=2)
    for path in range(X_test_shape[0]):
        p = []
        for step in range(X_test_shape[1]):
            if not all([ v == 0 for v in y[path][step]]):
                r = argmax(predictions[path][step])
                p.append(r)
        t = []
        for step in range(X_test_shape[1]):
            if not all([ v == 0 for v in y[path][step]]):
                r = argmax(y[path][step])
                t.append(r)
        if p == t:
            testOK += 1
        else:
            errs = 0
            for i in range(len(p)):
                if p[i] != t[i]:
                    errs += 1
            testErrors += errs
        testTotal += len(p)

# results to nestingbirds_rnn_results.txt
print("Train correct paths/total = ", trainOK, "/", X_train_shape[0], sep='', end='')
if X_train_shape[0] > 0:
    r = (float(trainOK) / float(X_train_shape[0])) * 100.0
    print(" (", str(round(r, 2)), "%)", sep='', end='')
print(", prediction errors/total = ", trainErrors, "/", trainTotal, sep='', end='')
trainErrorPct=0
if trainTotal > 0:
    trainErrorPct = (float(trainErrors) / float(trainTotal)) * 100.0
    print(" (", str(round(trainErrorPct, 2)), "%)", sep='', end='')
print('')
print("Test correct paths/total = ", testOK, "/", X_test_shape[0], sep='', end='')
if X_test_shape[0] > 0:
    r = (float(testOK) / float(X_test_shape[0])) * 100.0
    print(" (", str(round(r, 2)), "%)", sep='', end='')
print(", prediction errors/total = ", testErrors, "/", testTotal, sep='', end='')
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

