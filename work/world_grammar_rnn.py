# For conditions of distribution and use, see copyright notice in Mandala.java
# World grammar learning RNN.

from numpy import array, argmax
from keras.models import Sequential
from keras.layers import Dense
from keras.layers import TimeDistributed
from keras.layers import LSTM
import sys, getopt

# define LSTM configuration
n_neurons = 128
n_epochs = 1000

# results file name
results_filename = 'world_grammar_rnn_results.json'

# get options
try:
  opts, args = getopt.getopt(sys.argv[1:],"hn:e:",["neurons=","epochs="])
except getopt.GetoptError:
  print('world_grammar_rnn.py [-n <neurons>] [-e <epochs>]')
  sys.exit(1)
for opt, arg in opts:
  if opt == '-h':
     print('world_grammar_rnn.py [-n <neurons>] [-e <epochs>]')
     sys.exit()
  if opt in ("-n", "--neurons"):
     n_neurons = int(arg)
  elif opt in ("-e", "--epochs"):
     n_epochs = int(arg)

# import dataset
from world_path_rnn_dataset import X_train_shape, y_train_shape, X_train_seq, y_train_seq, X_test_shape, y_test_shape, X_test_seq, y_test_seq
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

# create LSTM
model = Sequential()
model.add(LSTM(n_neurons, input_shape=(X_train_shape[1], X_train_shape[2]), return_sequences=True))
model.add(TimeDistributed(Dense(y_train_shape[2])))
model.compile(loss='mean_squared_error', optimizer='adam')
#print(model.summary())

# train
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
    print('Path =', path, 'predictions: ', end='')
    p = []
    for step in range(X_train_shape[1]):
        r = argmax(predictions[path][step])
        p.append(r)
        print(terminals[r], ' ', sep='', end='')
    print('targets: ', end='')
    t = []
    for step in range(X_train_shape[1]):
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
        print('Errors = ', errs, '/', len(p), sep='', end='')
        if len(p) > 0:
            r = (float(errs) / float(len(p))) * 100.0
            print(" (", str(round(r, 2)), "%)", sep='', end='')
        print('')
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
    print('Path =', path, 'predictions: ', end='')
    p = []
    for step in range(X_test_shape[1]):
        r = argmax(predictions[path][step])
        p.append(r)
        print(terminals[r], ' ', sep='', end='')
    print('targets: ', end='')
    t = []
    for step in range(X_test_shape[1]):
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
        print('Errors = ', errs, '/', len(p), sep='', end='')
        if len(p) > 0:
            r = (float(errs) / float(len(p))) * 100.0
            print(" (", str(round(r, 2)), "%)", sep='', end='')
        print('')
    testTotal += len(p)

# Print results.
print("Train correct paths/total = ", trainOK, "/", X_train_shape[0], sep='', end='')
if X_train_shape[0] > 0:
    r = (float(trainOK) / float(X_train_shape[0])) * 100.0
    print(" (", str(round(r, 2)), "%)", sep='', end='')
print(", prediction errors/total = ", trainErrors, "/", trainTotal, sep='', end='')
if trainTotal > 0:
    r = (float(trainErrors) / float(trainTotal)) * 100.0
    print(" (", str(round(r, 2)), "%)", sep='', end='')
print('')
print("Test correct paths/total = ", testOK, "/", X_test_shape[0], sep='', end='')
if X_test_shape[0] > 0:
    r = (float(testOK) / float(X_test_shape[0])) * 100.0
    print(" (", str(round(r, 2)), "%)", sep='', end='')
print(", prediction errors/total = ", testErrors, "/", testTotal, sep='', end='')
if testTotal > 0:
    r = (float(testErrors) / float(testTotal)) * 100.0
    print(" (", str(round(r, 2)), "%)", sep='', end='')
print('')

# Write results to file.
with open(results_filename, 'w') as f:
    f.write('{')
    f.write('\"train_correct_paths\":\"'+str(trainOK)+'\",')
    f.write('\"train_total_paths\":\"'+str(X_train_shape[0])+'\",')
    f.write('\"train_prediction_errors\":\"'+str(trainErrors)+'\",')
    f.write('\"train_total_predictions\":\"'+str(trainTotal)+'\",')
    f.write('\"test_correct_paths\":\"'+str(testOK)+'\",')
    f.write('\"test_total_paths\":\"'+str(X_test_shape[0])+'\",')
    f.write('\"test_prediction_errors\":\"'+str(testErrors)+'\",')
    f.write('\"test_total_predictions\":\"'+str(testTotal)+'\"')
    f.write('}\n')

sys.exit(0)
