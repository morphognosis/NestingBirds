# For conditions of distribution and use, see copyright notice in Main.java
#
# Maze learning RNN.
from numpy import array, argmax
from keras.models import Sequential
from keras.layers import Dense
from keras.layers import TimeDistributed
from keras.layers import LSTM
import sys, getopt

# define LSTM configuration
n_neurons = 128
n_epochs = 1000

# get options
try:
  opts, args = getopt.getopt(sys.argv[1:],"hn:e:",["neurons=","epochs="])
except getopt.GetoptError:
  print('maze_rnn.py [-n <neurons>] [-e <epochs>]')
  sys.exit(2)
for opt, arg in opts:
  if opt == '-h':
     print('maze_rnn.py [-n <neurons>] [-e <epochs>]')
     sys.exit()
  if opt in ("-n", "--neurons"):
     n_neurons = int(arg)
  elif opt in ("-e", "--epochs"):
     n_epochs = int(arg)

# prepare data
from maze_dataset import X_train_shape, y_train_shape, X_train_seq, y_train_seq, X_test_shape, y_test_shape, X_test_seq, y_test_seq
seq = array(X_train_seq)
X = seq.reshape(X_train_shape[0], X_train_shape[1], X_train_shape[2])
seq = array(y_train_seq)
y = seq.reshape(y_train_shape[0], y_train_shape[1], y_train_shape[2])
print('X train:\n', X)
print('y train:\n', y)

# create LSTM
model = Sequential()
model.add(LSTM(n_neurons, input_shape=(X_train_shape[1], X_train_shape[2]), return_sequences=True))
model.add(TimeDistributed(Dense(y_train_shape[2])))
model.compile(loss='mean_squared_error', optimizer='adam')
print(model.summary())

# train
model.fit(X, y, epochs=n_epochs, batch_size=X_train_shape[0], verbose=2)

# evaluate
seq = array(X_train_seq)
X = seq.reshape(X_train_shape[0], X_train_shape[1], X_train_shape[2])
seq = array(y_train_seq)
y = seq.reshape(y_train_shape[0], y_train_shape[1], y_train_shape[2])
#print('X train:\n', X)
#print('y train:\n', y)
predictions = model.predict(X, batch_size=X_train_shape[0], verbose=0)
print('Train results:')
trainOK = 0
for seq in range(X_train_shape[0]):
    print('Maze =', seq, 'responses: ', end='')
    p = []
    for step in range(X_train_shape[1]):
        r = argmax(predictions[seq][step])
        p.append(r)
        print(r, ' ', sep='', end='')
    print('targets: ', end='')
    t = []
    for step in range(X_train_shape[1]):
        r = argmax(y[seq][step])
        t.append(r)
        print(r, ' ', sep='', end='')
    if p == t:
        print('OK')
        trainOK += 1
    else:
        print('Error')

seq = array(X_test_seq)
X = seq.reshape(X_test_shape[0], X_test_shape[1], X_test_shape[2])
seq = array(y_test_seq)
y = seq.reshape(y_test_shape[0], y_test_shape[1], y_test_shape[2])
#print('X test:\n', X)
#print('y test:\n', y)
predictions = model.predict(X, batch_size=X_test_shape[0], verbose=0)
print('Test results:')
testOK = 0
for seq in range(X_test_shape[0]):
    print('Maze =', seq, 'responses: ', end='')
    p = []
    for step in range(X_test_shape[1]):
        r = argmax(predictions[seq][step])
        p.append(r)
        print(r, ' ', sep='', end='')
    print('targets: ', end='')
    t = []
    for step in range(X_test_shape[1]):
        r = argmax(y[seq][step])
        t.append(r)
        print(r, ' ', sep='', end='')
    if p == t:
        print('OK')
        testOK += 1
    else:
        print('Error')

# Print results.
print("Train correct/total = ", trainOK, "/", X_train_shape[0], sep='', end='')
if X_train_shape[0] > 0:
    print(" (", ((float(trainOK) / float(X_train_shape[0])) * 100.0), "%)", sep='', end='')
print('')
print("Test correct/total = ", testOK, "/", X_test_shape[0], sep='', end='')
if X_test_shape[0] > 0:
    print(" (", ((float(testOK) / float(X_test_shape[0])) * 100.0), "%)", sep='', end='')
print('')