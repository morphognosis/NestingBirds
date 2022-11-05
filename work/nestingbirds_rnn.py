# Nesting birds RNN.
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
  print('nestingbirds_rnn.py [-n <neurons>] [-e <epochs>]')
  sys.exit(2)
for opt, arg in opts:
  if opt == '-h':
     print('nestingbirds_rnn.py [-n <neurons>] [-e <epochs>]')
     sys.exit()
  if opt in ("-n", "--neurons"):
     n_neurons = int(arg)
  elif opt in ("-e", "--epochs"):
     n_epochs = int(arg)

# prepare data
from nestingbirds_dataset import X_train_shape, X_train_seq, y_train_shape, y_train_seq
seq = array(X_train_seq)
X = seq.reshape(X_train_shape[0], X_train_shape[1], X_train_shape[2])
seq = array(y_train_seq)
y = seq.reshape(y_train_shape[0], y_train_shape[1], y_train_shape[2])
print('X train:\n', X)
print('y:\n', y)

# create LSTM
model = Sequential()
model.add(LSTM(n_neurons, input_shape=(X_train_shape[1], X_train_shape[2]), return_sequences=True))
model.add(TimeDistributed(Dense(y_train_shape[2])))
model.compile(loss='mean_squared_error', optimizer='adam')
print(model.summary())

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

print("Train correct paths/total = ", trainOK, "/", X_train_shape[0], sep='', end='')
if X_train_shape[0] > 0:
    r = (float(trainOK) / float(X_train_shape[0])) * 100.0
    print(" (", str(round(r, 2)), "%)", sep='', end='')
print(", prediction errors/total = ", trainErrors, "/", trainTotal, sep='', end='')
trainErrorPct=0
if trainTotal > 0:
    trainErrorPct = (float(trainErrors) / float(trainTotal)) * 100.0
    print(" (", str(round(trainErrorPct, 2)), "%)", sep='')
original_stdout = sys.stdout
with open('nestingbirds_rnn_results.txt', 'w') as f:
    sys.stdout = f
    print("Train correct paths/total = ", trainOK, "/", X_train_shape[0], sep='', end='')
    if X_train_shape[0] > 0:
        r = (float(trainOK) / float(X_train_shape[0])) * 100.0
        print(" (", str(round(r, 2)), "%)", sep='', end='')
    print(", prediction errors/total = ", trainErrors, "/", trainTotal, sep='', end='')
    trainErrorPct=0
    if trainTotal > 0:
        trainErrorPct = (float(trainErrors) / float(trainTotal)) * 100.0
        print(" (", str(round(trainErrorPct, 2)), "%)", sep='')
    sys.stdout = original_stdout