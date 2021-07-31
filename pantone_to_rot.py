import os
import pandas as pd
import numpy as np

with open('messdaten/csv/pantone.txt') as file:
    karten = [[x[0:2], x[2:4]] for x in file.read().split('\n')]

currentHelligkeit = 1
data = {}
index = 0

for karte in karten:
    if karte[0] not in data:
        data[karte[0]] = []
    data[karte[0]].append(karte[0] + karte[1])

print(data['5R'])
print(data['4R'])
print(data['3R'])
print(data['2R'])
print(data['1R'])
print(data['1Y'])
print(data['2Y'])
print(data['3Y'])
print(data['4Y'])
print(data['5Y'])
