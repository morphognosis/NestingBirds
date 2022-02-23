# Caustion coder.
# Map input cause to output effect, encoding relationship in the process.
# ref: https://blog.keras.io/building-autoencoders-in-keras.html

import keras
from keras import layers

# Sizes of cause, code, and effect.
cause_dim = 16
code_dim = 8
effect_dim = 16

# Hidden layer dimensions.
hidden_dim = 128

# Causations.
num_causations = 2
num_cause_features = 2
num_effect_features = 2

# Dataset size.
dataset_size = num_causations + 1

# Cause layer.
cause_layer = keras.Input(shape=(cause_dim,))

# Hidden input layers.
encoded = layers.Dense(hidden_dim, activation='relu')(cause_layer)
encoded = layers.Dense(hidden_dim, activation='relu')(encoded)

# Code layer with an L1 activity regularizer.
from keras import regularizers
code_layer = layers.Dense(code_dim, activation='relu',
                activity_regularizer=regularizers.l1(10e-5))(encoded)

# Hidden output layers.
decoded = layers.Dense(hidden_dim, activation='relu')(code_layer)
decoded = layers.Dense(hidden_dim, activation='relu')(decoded)

# Effect layer.
effect_layer = layers.Dense(effect_dim, activation='sigmoid')(decoded)

# Causation model maps cause to effect.
causation_model = keras.Model(cause_layer, effect_layer)

# Coder model maps cause to its code.
coder_model = keras.Model(cause_layer, code_layer)

# Compile model with a mean squared error loss and Adam optimizer.
causation_model.compile(optimizer='adam', loss='mse')

# Generate cause and effect pairs.
# off=0.0, on=1.0
import numpy as np
cause_data = np.zeros((dataset_size, cause_dim))
effect_data = np.zeros((dataset_size, effect_dim))
import random
cause_feature_idxs = []
for _ in range(num_causations):
    while True:
        idxs = random.sample(range(0, cause_dim), num_cause_features)
        idxs.sort()
        found = False
        for i in cause_feature_idxs:
            if len(list(set(idxs) & set(i))) > 0:
                found = True
                break
        if found == False:
            cause_feature_idxs.append(idxs)
            break
effect_feature_idxs = []
for _ in range(num_causations):
    while True:
        idxs = random.sample(range(0, effect_dim), num_effect_features)
        idxs.sort()
        found = False
        for i in effect_feature_idxs:
            if len(list(set(idxs) & set(i))) > 0:
                found = True
                break
        if found == False:
            effect_feature_idxs.append(idxs)
            break
for row in range(dataset_size):
    idxs = [row]
    if row >= num_causations:
        idxs = []
        for i in range(num_causations):
            idxs.append(i)
    for i in idxs:
        for j in range(len(cause_feature_idxs[i])):
            idx = cause_feature_idxs[i][j]
            cause_data[row,idx] = 1.0
        for j in range(len(effect_feature_idxs[i])):
            idx = effect_feature_idxs[i][j]
            effect_data[row,idx] = 1.0

# Train causation model.
causation_model.fit(cause_data, effect_data,
                epochs=100,
                batch_size=256,
                shuffle=True)

# Predict effects from causes.
print('predictions:')
for i in range(num_causations):
    print('causation=',i,':',sep='')
    print('cause feature indexes:', cause_feature_idxs[i])
    print('effect feature indexes:', effect_feature_idxs[i])
    cause_instance = np.array([cause_data[i]])
    coded_instance = coder_model.predict(cause_instance)
    predicted_instance = causation_model.predict(cause_instance)
    print('cause:', cause_instance)
    print('code:', coded_instance)
    print('effect:', np.array([effect_data[i]]))
    print('prediction:', predicted_instance)
i = num_causations
print('causation=all:')
cause_instance = np.array([cause_data[i]])
coded_instance = coder_model.predict(cause_instance)
predicted_instance = causation_model.predict(cause_instance)
print('cause:', cause_instance)
print('code:', coded_instance)
print('effect:', np.array([effect_data[i]]))
print('prediction:', predicted_instance)



