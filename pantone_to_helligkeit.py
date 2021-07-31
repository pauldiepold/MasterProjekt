import os
import pandas as pd
import numpy as np

with open('messdaten/csv/pantone.txt') as file:
    karten = [[x[0:2], x[2:4]] for x in file.read().split('\n')]

currentHelligkeit = 1
data = []
index = 0

for karte in karten:
    if currentHelligkeit != int(karte[1]):
        print(data)
        data = []
        currentHelligkeit = int(karte[1])
    data.append(karte[0] + karte[1])
print(data)
