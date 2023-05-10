# Nesting birds RNN.
# results written to nestingbirds_rnn_test_results.txt
from numpy import array, argmax
from keras.models import Sequential
from keras.layers import Dense
from keras.layers import TimeDistributed
from keras.layers import LSTM
from keras.models import load_model
import sys

# results file name
results_filename = 'nestingbirds_rnn_test_results.json'

# load model
model = load_model('nestingbirds_rnn.model')

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

# results to nestingbirds_rnn_test_results.txt
print("Test correct paths/total = ", testOK, "/", X_test_shape[0], sep='', end='')
if X_test_shape[0] > 0:
    r = (float(testOK) / float(X_test_shape[0])) * 100.0
    print(" (", str(round(r, 2)), "%)", sep='', end='')
print(", prediction errors/total = ", testErrors, "/", testTotal, sep='', end='')
testErrorPct=0
if testTotal > 0:
    testErrorPct = (float(testErrors) / float(testTotal)) * 100.0
    print(" (", str(round(testErrorPct, 2)), "%)", sep='', end='')
else:
    print('')

# Write results to file.
with open(results_filename, 'w') as f:
    f.write('{')
    f.write('\"test_prediction_errors\":\"'+str(testErrors)+'\",')
    f.write('\"test_total_predictions\":\"'+str(testTotal)+'\",')
    f.write('\"test_error_pct\":\"'+str(round(testErrorPct, 2))+'\"')
    f.write('}\n')

