# For conditions of distribution and use, see copyright notice in Mandala.java
# World path NN.

from numpy import array, argmax
from numpy import loadtxt
from keras.models import Sequential
from keras.layers import Dense
import sys, getopt

# parameters
n_neurons = 128
n_epochs = 150

# results file name
results_filename = 'world_path_nn_results.json'

# get options
try:
  opts, args = getopt.getopt(sys.argv[1:],"hn:e:",["neurons=","epochs="])
except getopt.GetoptError:
  print('world_path_nn.py [-n <hidden_neurons>] [-e <epochs>]')
  sys.exit(1)
for opt, arg in opts:
  if opt == '-h':
     print('world_path_nn.py [-n <hidden_neurons>] [-e <epochs>]')
     sys.exit(0)
  if opt in ("-n", "--neurons"):
     n_neurons = int(arg)
  elif opt in ("-e", "--epochs"):
     n_epochs = int(arg)
  else:
     print('world_path_nn.py [-n <hidden_neurons>] [-e <epochs>]')
     sys.exit(1)

# import dataset
from world_path_nn_dataset import X_train_shape, y_train_shape, X_train, y_train, X_test_shape, y_test_shape, X_test, y_test
if X_train_shape[0] == 0:
    print('Empty train dataset')
    sys.exit(1)
if X_test_shape[0] == 0:
    print('Empty test dataset')
    sys.exit(1)
seq = array(X_train)
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

# validate
seq = array(X_train)
X = seq.reshape(X_train_shape[0], X_train_shape[1])
seq = array(y_train)
y = seq.reshape(y_train_shape[0], y_train_shape[1])
predictions = model.predict(X, batch_size=X_train_shape[0], verbose=2)
trainOK = 0
trainErrors = 0
trainTotal = 0
for path in range(X_train_shape[0]):
    p = []
    for step in range(y_train_shape[1]):
        r = argmax(predictions[path][step])
        p.append(r)
    t = []
    for step in range(y_train_shape[1]):
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
seq = array(X_test)
X = seq.reshape(X_test_shape[0], X_test_shape[1])
seq = array(y_test)
y = seq.reshape(y_test_shape[0], y_test_shape[1])
predictions = model.predict(X, batch_size=X_test_shape[0], verbose=2)
testOK = 0
testErrors = 0
testTotal = 0
for path in range(X_test_shape[0]):
    p = []
    for step in range(y_test_shape[1]):
        r = argmax(predictions[path][step])
        p.append(r)
    t = []
    for step in range(y_test_shape[1]):
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

# Print results.
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
    f.write('\"train_correct_paths\":\"'+str(trainOK)+'\",')
    f.write('\"train_total_paths\":\"'+str(X_train_shape[0])+'\",')
    f.write('\"train_prediction_errors\":\"'+str(trainErrors)+'\",')
    f.write('\"train_total_predictions\":\"'+str(trainTotal)+'\",')
    f.write('\"train_error_pct\":\"'+str(round(trainErrorPct, 2))+'\",')
    f.write('\"test_correct_paths\":\"'+str(testOK)+'\",')
    f.write('\"test_total_paths\":\"'+str(X_test_shape[0])+'\",')
    f.write('\"test_prediction_errors\":\"'+str(testErrors)+'\",')
    f.write('\"test_total_predictions\":\"'+str(testTotal)+'\",')
    f.write('\"test_error_pct\":\"'+str(round(testErrorPct, 2))+'\"')
    f.write('}\n')


sys.exit(0)
