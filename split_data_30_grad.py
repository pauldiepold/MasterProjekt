import pandas as pd
import numpy as np

with open('messdaten/csv/pantone.txt') as file:
    karten = [[x[0:2], x[2:4]] for x in file.read().split('\n')]

currentFile = ''
currentFileName = ''
index = 0

for karte in karten:
    if currentFileName != karte[1]:
        currentFileName = karte[1]
        index = 0
        currentFile = pd.read_csv("messdaten/csv/farbkarten_30_grad/" + currentFileName + ".csv")

    data = np.asmatrix(currentFile.values)[(0 + 10 * index):(10 + 10 * index), :]

    df = pd.DataFrame(data,
                      columns=['Temp', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'R', 'S',
                               'T', 'U', 'V', 'W'])

    df.to_csv("messdaten/csv/farbkarten_getrennt_30_grad/" + karte[0] + karte[1] + '.csv', index=False)
    index += 1

