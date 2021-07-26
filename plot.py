import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import tensorflow as tf
import os
import fileinput
from sklearn.metrics import confusion_matrix
import seaborn as sns
from tensorflow.keras import datasets, layers, models, optimizers
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Dense, Dropout, Activation, Flatten
# from tensorflow.keras.layers import Conv1D, MaxPooling1D
# more info on callbakcs: https://keras.io/callbacks/ model saver is cool too.
from tensorflow.keras.callbacks import TensorBoard
import pickle
import time
import itertools

print(f"TensorFlow version = {tf.__version__}\n")

# Set a fixed random seed value, for reproducibility, this will allow us to get
# the same random numbers each time the notebook is run
SEED = 1332
np.random.seed(SEED)
tf.random.set_seed(SEED)

CLASSES = [];

for file in os.listdir("./" + folder + "/"):
    if file.endswith(".csv"):
        CLASSES.append(os.path.splitext(file)[0])

CLASSES.sort()
NUM_CLASSES = len(CLASSES)

# create a one-hot encoded matrix that is used in the output
ONE_HOT_ENCODED_CLASSES = np.eye(NUM_CLASSES)

inputs = np.array([]).reshape(0, 19)
outputs = np.array([]).reshape(0, NUM_CLASSES)

# read each csv file and push an input and output
for class_index in range(NUM_CLASSES):
    objectClass = CLASSES[class_index]
    df = pd.read_csv("./" + folder + "/" + objectClass + ".csv")
    columns = list(df)
    # get rid of pesky empty value lines of csv which cause NaN inputs to TensorFlow
    df = df.dropna()
    df = df.reset_index(drop=True)

    # calculate the number of objectClass recordings in the file
    num_recordings = int(df.shape[0])

    input = df.to_numpy()
    output = np.repeat([ONE_HOT_ENCODED_CLASSES[class_index]], num_recordings, axis=0)

    inputs = np.concatenate((inputs, input), axis=0)
    outputs = np.concatenate((outputs, output), axis=0)

    print(f"\u001b[32;4m{objectClass}\u001b[0m class will be output \u001b[32m{class_index}\u001b[0m of the classifier")
    print(f"{num_recordings} samples captured for training with inputs {list(df)} \n")

# Normalisierung der Temperatur
inputs[:, 0] = np.divide(inputs[:, 0], 60)

max_wert = input.max()

# Normalisierung der Sensordaten
if normalize == 'absolut':
    inputs[:, 1:19] = np.divide(inputs[:, 1:19], max_wert)
if normalize == 'relativ':
    def normalize_relativ(row):
        return np.divide(row, row.sum())


    inputs[:, 1:19] = np.apply_along_axis(normalize_relativ, axis=1, arr=inputs[:, 1:19])

# inputs = np.concatenate((inputs, inputs), axis=0)
# inputs = np.concatenate((inputs, inputs), axis=0)
# outputs = np.concatenate((outputs, outputs), axis=0)
# outputs = np.concatenate((outputs, outputs), axis=0)

print("Data set parsing and preparation complete.")

# Randomize the order of the inputs, so they can be evenly distributed for training, testing, and validation
# https://stackoverflow.com/a/37710486/2020087
num_inputs = len(inputs)
randomize = np.arange(num_inputs)
np.random.shuffle(randomize)

# Swap the consecutive indexes (0, 1, 2, etc) with the randomized indexes
inputs = inputs[randomize]
outputs = outputs[randomize]

# Split the recordings (group of samples) into three sets: training, testing and validation
TRAIN_SPLIT = int(0.6 * num_inputs)
TEST_SPLIT = int(0.2 * num_inputs + TRAIN_SPLIT)

inputs_train, inputs_validate, inputs_test = np.split(inputs, [TRAIN_SPLIT, TEST_SPLIT])
outputs_train, outputs_validate, outputs_test = np.split(outputs, [TRAIN_SPLIT, TEST_SPLIT])

print("Data set randomization and splitting complete.")
