from htmd.ui import *
from keras.models import Sequential
from keras.layers import Convolution3D, MaxPooling3D, Dropout, Flatten, Dense, Activation
from keras.layers.advanced_activations import ELU
from keras.optimizers import Adam

# Parse the PDB data
mol = Molecule('4NIE')
print(mol)

# The DeepSite DCNN
boxsize = 16
model = Sequential()
model.add(Convolution3D(32, (8, 8, 8), padding='same',
                        input_shape=(boxsize, boxsize, boxsize, 8)))
model.add(ELU())
model.add(Convolution3D(48, (4, 4, 4)))
model.add(ELU())
model.add(MaxPooling3D(pool_size=(2, 2, 2)))
model.add(Dropout(0.25))

model.add(Convolution3D(64, (4, 4, 4), padding='same'))
model.add(ELU())
model.add(Convolution3D(96, (4, 4, 4)))
model.add(ELU())
model.add(MaxPooling3D(pool_size=(2, 2, 2)))
model.add(Dropout(0.25))

model.add(Flatten())
model.add(Dense(256))
model.add(ELU())
model.add(Dropout(0.5))
model.add(Dense(1))
model.add(Activation('sigmoid'))

opt = Adam()
model.compile(loss='binary_crossentropy', optimizer=opt, metrics=['accuracy'])
