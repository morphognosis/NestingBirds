# For conditions of distribution and use, see copyright notice in Mandala.java
# World grammar learning TDNN.

from numpy import array, argmax
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense
from sklearn.datasets import make_regression
import numpy as np

# Default parameters.
n_neurons = 50
n_epochs = 1000
results_filename = 'world_grammar_tdnn_results.json'

# Get options.
import getopt
import sys
usage = 'world_grammar_tdnn.py [--neurons <number of neurons>] [--epochs <number of epochs>] [--results_filename <filename>]'
try:
  opts, args = getopt.getopt(sys.argv[1:],"h",["help","neurons=","epochs=","results_filename="])
except getopt.GetoptError:
  print(usage)
  sys.exit(1)
for opt, arg in opts:
  if opt == '-h' or opt == '--help':
     print(usage)
     sys.exit(0)
  if opt == "--neurons":
     n_neurons = int(arg)
  elif opt == "--epochs":
     n_epochs = int(arg)
  elif opt == "--results_filename":
     results_filename = arg
  else:
     print(usage)
     sys.exit(1)

# Import dataset.
from world_path_tdnn_dataset import X_train_shape, y_train_shape, X_train_seq, y_train_seq, X_test_shape, y_test_shape, X_test_seq, y_test_seq
if X_train_shape[0] == 0:
    print('Empty train dataset')
    sys.exit(1)
if X_test_shape[0] == 0:
    print('Empty test dataset')
    sys.exit(1)
seq = array(X_train_seq)
X = seq.reshape(X_train_shape[0], X_train_shape[1], X_train_shape[2])
seq = array(y_train_seq)
y = seq.reshape(y_train_shape[0], y_train_shape[1], y_train_shape[2])

# Create model.
model = Sequential()
model.add(Dense(n_neurons, input_shape=(X_train_shape[1], X_train_shape[2]), activation='relu'))
model.add(Dense(n_neurons, activation='relu'))
model.add(Dense(y_train_shape[2], activation='linear'))
model.compile(loss='mse', optimizer='adam')
#print(model.summary())

# Train model.
model.fit(X, y, epochs=n_epochs, batch_size=X_train_shape[0], verbose=2)
#model.save("model.dat")

# evaluate training
seq = array(X_train_seq)
X = seq.reshape(X_train_shape[0], X_train_shape[1], X_train_shape[2])
seq = array(y_train_seq)
y = seq.reshape(y_train_shape[0], y_train_shape[1], y_train_shape[2])
predictions = model.predict(X, batch_size=X_train_shape[0], verbose=2)
terminals = ['a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z']
print('Train results:')
trainOK = 0
trainErrors = 0
trainTotal = 0
for path in range(X_train_shape[0]):
    print('Sequence =', path, 'prediction: ', end='')
    p = []
    for step in range(y_train_shape[1]):
        r = argmax(predictions[path][step])
        p.append(r)
        print(terminals[r], ' ', sep='', end='')
    print('target: ', end='')
    t = []
    for step in range(y_train_shape[1]):
        r = argmax(y[path][step])
        t.append(r)
        print(terminals[r], ' ', sep='', end='')
    if p == t:
        print('OK')
        trainOK += 1
    else:
        errs = 0
        for i in range(len(p)):
            if p[i] != t[i]:
                errs += 1
        trainErrors += errs
        print('Error')
    trainTotal += len(p)

# predict
seq = array(X_test_seq)
X = seq.reshape(X_test_shape[0], X_test_shape[1], X_test_shape[2])
seq = array(y_test_seq)
y = seq.reshape(y_test_shape[0], y_test_shape[1], y_test_shape[2])
predictions = model.predict(X, batch_size=X_test_shape[0], verbose=2)
print('Test results:')
testOK = 0
testErrors = 0
testTotal = 0
for path in range(X_test_shape[0]):
    print('Sequence =', path, 'prediction: ', end='')
    p = []
    for step in range(y_test_shape[1]):
        r = argmax(predictions[path][step])
        p.append(r)
        print(terminals[r], ' ', sep='', end='')
    print('target: ', end='')
    t = []
    for step in range(y_test_shape[1]):
        r = argmax(y[path][step])
        t.append(r)
        print(terminals[r], ' ', sep='', end='')
    if p == t:
        print('OK')
        testOK += 1
    else:
        errs = 0
        for i in range(len(p)):
            if p[i] != t[i]:
                errs += 1
        testErrors += errs
        print('Error')
    testTotal += len(p)

# Print results.
print("Train prediction errors/total = ", trainErrors, "/", trainTotal, sep='', end='')
if trainTotal > 0:
    r = (float(trainErrors) / float(trainTotal)) * 100.0
    print(" (", str(round(r, 2)), "%)", sep='', end='')
print('')
print("Test prediction errors/total = ", testErrors, "/", testTotal, sep='', end='')
if testTotal > 0:
    r = (float(testErrors) / float(testTotal)) * 100.0
    print(" (", str(round(r, 2)), "%)", sep='', end='')
print('')

# Write results to file.
with open(results_filename, 'w') as f:
    f.write('{')
    f.write('\"train_prediction_errors\":\"'+str(trainErrors)+'\",')
    f.write('\"train_total_predictions\":\"'+str(trainTotal)+'\",')
    f.write('\"test_prediction_errors\":\"'+str(testErrors)+'\",')
    f.write('\"test_total_predictions\":\"'+str(testTotal)+'\"')
    f.write('}\n')

sys.exit(0)
