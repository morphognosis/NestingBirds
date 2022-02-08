# Caustion coder.
# Maps cause input to effect output, encoding relationship in the process.
# ref: https://blog.keras.io/building-autoencoders-in-keras.html

import keras
from keras import layers

# Sizes of cause, code, and effect.
cause_dim = 64
code_dim = 16
effect_dim = 64

# Hidden layer dimensions.
hidden_dim = 128

# Number of causations.
num_causations = 10

# Cause layer.
cause_layer = keras.Input(shape=(cause_dim,))

# Hidden input layers.
encoded = layers.Dense(hidden_dim, activation='tanh')(cause_layer)
encoded = layers.Dense(hidden_dim, activation='tanh')(encoded)

# Code layer with an L1 activity regularizer.
from keras import regularizers
code_layer = layers.Dense(code_dim, activation='tanh',
                activity_regularizer=regularizers.l1(10e-5))(encoded)

# Hidden output layers.
decoded = layers.Dense(hidden_dim, activation='tanh')(code_layer)
decoded = layers.Dense(hidden_dim, activation='tanh')(decoded)

# Effect layer.
effect_layer = layers.Dense(effect_dim, activation='tanh')(decoded)

# Causation model maps cause to effect.
causation_model = keras.Model(cause_layer, effect_layer)

# Coder model maps cause to its code.
coder_model = keras.Model(cause_layer, code_layer)

# Compile model with a mean squared error loss and Adam optimizer.
causation_model.compile(optimizer='adam', loss='mse')

# Generate cause and effect pairs.
# off=-1.0, on=1.0
import numpy as np
cause_data = np.random.uniform(-1.0, 1.0, size = (num_causations, cause_dim))
for row in range(num_causations):
    for col in range(cause_dim):
       if cause_data[row,col] >= 0.0:
           cause_data[row,col] = 1.0
       else:
           cause_data[row,col] = -1.0
effect_data = np.random.uniform(-1.0, 1.0, size = (num_causations, effect_dim))
for row in range(num_causations):
    for col in range(effect_dim):
       if effect_data[row,col] >= 0.0:
           effect_data[row,col] = 1.0
       else:
           effect_data[row,col] = -1.0

# Train causation model.
causation_model.fit(cause_data, effect_data,
                epochs=100,
                batch_size=256,
                shuffle=True)

# Predict effects from causes.
coded_data = coder_model.predict(cause_data)
predicted_data = causation_model.predict(cause_data)
print('cause_data', cause_data)
print('coded_data', coded_data)
print('effect_data', effect_data)
print('predicted_data', predicted_data)




