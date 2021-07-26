import os
import pandas as pd
import numpy as np

with open('pantone.txt') as file:
    karten = [[x[0:2], x[2:4]] for x in file.read().split('\n')]

currentFile = ''
currentFileName = ''
index = 0

for karte in karten:
    if currentFileName != karte[1]:
        currentFileName = karte[1]
        index = 0
        currentFile = pd.read_csv("messdaten/csv/farbkarten/" + currentFileName + ".csv")

    df = pd.DataFrame(np.asmatrix(currentFile.values)[(0 + 20 * index):(20 + 20 * index), :],
                      columns=['Temp', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'R', 'S',
                               'T', 'U', 'V', 'W'])

    df.to_csv("messdaten/csv/farbkarten_getrennt/" + karte[1] + karte[0] + '.csv', index=False)
    index += 1

