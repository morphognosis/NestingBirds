# Sensor discriminator.
# Detect significant features in input sensors.
# ref: "How to measure importance of inputs" by Warren S. Sarle, SAS Institute Inc., Cary, NC, USA 
#      ftp://ftp.sas.com/pub/neural/importance.html

import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'
import tensorflow as tf
from tensorflow import keras
from keras import layers
from keras import regularizers
import numpy as np
import random
import sys, getopt

# Sensor files.
sensor_input_filename = ''
sensor_output_filename = ''

# Dimensions.
input_dim = -1
output_dim = -1
hidden_dim = 256

# Dataset size.
dataset_size = -1

# Learning rate.
learning_rate = 0.01

# Training epochs.
epochs = 100

# Random seed.
random_seed = 4517

# Verbose.
verbose = True

usage = 'sensor_discriminator.py --sensor_input_file <input file name> --sensor_output_file <output file name> [--network_hidden_dimension <hidden layer dimension>] [--learning_rate <learning rate>] [--epochs <training epochs>] [--random_seed <random seed>] [--verbose <true | false>]'
try:
  opts, args = getopt.getopt(sys.argv[1:],"i:o:s:h:l:e:r:v:?",["sensor_input_file=","sensor_output_file=","network_hidden_dimension=","learning_rate=","epochs=","random_seed=","verbose=","help"])
except getopt.GetoptError:
  print(usage)
  sys.exit(1)
for opt, arg in opts:
  if opt in ("-?", "--help"):
     print(usage)
     sys.exit(0)
  if opt in ("-i", "--sensor_input_file"):
     sensor_input_filename = arg
  elif opt in ("-o", "--sensor_output_file"):
     sensor_output_filename = arg
  elif opt in ("-h", "--network_hidden_dimension"):
     hidden_dim = int(arg)
  elif opt in ("-l", "--learning_rate"):
     learning_rate = float(arg)
  elif opt in ("-e", "--epochs"):
     epochs = int(arg)
  elif opt in ("-r", "--random_seed"):
     random_seed = int(arg)
  elif opt in ("-v", "--verbose"):
     if arg == 'true':
         verbose = True
     elif arg == 'false':
         verbose = False
     else:
         print(usage)
         sys.exit(1)
  else:
     print(usage)
     sys.exit(1)

if sensor_input_filename == '':
   print('sensor_input_file is required')
   print(usage)
   sys.exit(1)

if sensor_output_filename == '':
   print('sensor_output_file is required')
   print(usage)
   sys.exit(1)

# Seed random numbers.
random.seed(random_seed)

# Load input sensors.
data = np.loadtxt(sensor_input_filename, delimiter=',')
if len(data) == 0:
   print('sensor_input_file empty')
   sys.exit(1)
if isinstance(data[0], np.ndarray) == False:
   d = np.zeros((1, len(data)))
   d[0] = data
   data = d
data = np.unique(data, axis=0)
dataset_size = len(data)
if dataset_size == 0:
   print('sensor_input_file empty')
   sys.exit(1)
input_data = np.zeros((dataset_size, len(data[0]) - 1))
output_vals = []
for i in range(dataset_size):
    input_data[i] = data[i][:-1]
    output_vals.append((int)(data[i][-1]))
input_dim = len(input_data[0])
output_dim = max(output_vals) + 1
output_data = np.zeros((dataset_size, output_dim))
for i in range(dataset_size):
    output_data[i][output_vals[i]] = 1.0

# Input layer.
input_layer = keras.Input(shape=(input_dim,))

# Hidden layer with an L1 activity regularizer.
hidden_layer = layers.Dense(hidden_dim, activation='relu',
                activity_regularizer=regularizers.l1(10e-5))(input_layer)

# Output layer.
output_layer = layers.Dense(output_dim, activation='sigmoid')(hidden_layer)

# Model maps input to output.
model = keras.Model(input_layer, output_layer)

# Compile model with a mean squared error loss and Adam optimizer.
model.compile(optimizer=keras.optimizers.Adam(learning_rate), loss='mse')

# Train model.
if verbose == True:
    model.fit(input_data, output_data,
                    epochs=epochs,
                    batch_size=256,
                    shuffle=True,
                    verbose=2)
else:
    model.fit(input_data, output_data,
                    epochs=epochs,
                    batch_size=256,
                    shuffle=True,
                    verbose=0)

# Discriminate sensors.
results = []
correct = 0
for i in range(dataset_size):
    input_sensors = np.array([input_data[i]])
    prediction = model.predict(input_sensors)
    if verbose == True:
        print('input:', input_sensors[0])
        print('expected:', output_data[i])
        print('prediction:', prediction[0])
    if np.argmax(output_data[i]) == np.argmax(prediction[0]):
        correct += 1
        maxOutIdx = np.argmax(prediction[0])
        maxOutVal = np.max(prediction[0])
        if maxOutVal > 0.0:
            if verbose == True:
                maxInIdx = -1
                maxInDelta = 0.0
                for j in range(input_dim):
                    v = input_sensors[0][j]
                    if v == 1.0:
                        input_sensors[0][j] = 0.0
                        prediction = model.predict(input_sensors)
                        input_sensors[0][j] = v
                        print('index=',j,' prediction:', prediction[0])
                        d = maxOutVal - prediction[0][maxOutIdx]
                        if d > maxOutVal:
                           d = maxOutVal
                        if d > 0.0 and (maxInIdx == -1 or d > maxInDelta):
                            maxInIdx = j
                            maxInDelta = d
                print('maxInIdx=', maxInIdx, 'maxInDelta=', maxInDelta)
            discriminated_input = []
            count = 0
            for j in range(input_dim):
                v = input_sensors[0][j]
                if v == 1.0:
                    input_sensors[0][j] = 0.0
                    prediction = model.predict(input_sensors)
                    input_sensors[0][j] = 1.0
                    if maxOutVal > prediction[0][maxOutIdx]:
                        discriminated_input.append(1.0)
                        count = count + 1
                    else:
                         discriminated_input.append(-1.0)
                else:
                     discriminated_input.append(-1.0)
            if count > 0:
                add = True
                for j in range(len(results)):
                    duplicate = True
                    for k in range(input_dim):
                        if discriminated_input[k] != results[j][k]:
                            duplicate = False
                            break
                    if duplicate == True:
                        add = False
                        break
                if add == True:
                    results.append(discriminated_input)
                    if verbose == True:
                       print('discriminated input:', discriminated_input)
if verbose == True:
    print('results:')
    print(results)
    print('correct=', correct, '/', dataset_size, ' (', ((correct / dataset_size) * 100.0), '%)', sep='')
with open(sensor_output_filename, "wt") as f:
    for i in range(len(results)):
        n = len(results[i])
        for j in range(n):
            f.write(str(results[i][j]))
            if j == n - 1:
                f.write('\n')
            else:
                f.write(',')

sys.exit(0)

