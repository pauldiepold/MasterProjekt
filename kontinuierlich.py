# 1: Start program on create.arduino.cc -> keep it running infinitely
# 2: Make sure serial port is not used -> Monitor needs to be closed
# 3: Match port and baud-rate

import serial
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import openpyxl
import os

port = 'COM6'  # Port needs to match -> check in create.arduino.cc
baud = 115200  # baud-rate needs to match -> check in create.arduino.cc

n = 5  # Number of read-cycles
filename = 'kontinuierlich/alle'
normalize = False
append = True


try:
    ser = serial.Serial(port, baud)
    ser.close()
except OSError:
    print("The number of serial may be not right or the connection is not built correctly.\n")

ser = serial.Serial(port, baud)
ser.timeout = 2  # waiting time for reading

data = []

for i in range(n):
    line = [float(x) for x in ser.readline().decode().split(",")]  # read next line, split string and parse to float

    if len(line) <= 1:
        print("No more data from Ardunio...")
        break

    if normalize:
        sensor_sum = sum(line[1:18])
        line[1:18] = [x / sensor_sum for x in line[1:18]]
        line[0] = round((line[0] - 10) / 40, 2)

    print(line)
    print(str(i + 1) + ' von ' + str(n))

    data.append(line)

ser.close()
df = pd.DataFrame(data, columns=['Temp', 'I', 'J', 'K', 'L', 'R', 'S'])

# # Visualisierung mittelwert bilden
# plt.plot(df.mean()[3:])
# plt.title('Temp: ' + str(round(df.mean()[1], 1)))


# Create dirs if they don't exist
path = os.getcwd()
if not os.path.exists(path + '/messdaten'):
    os.mkdir(path + '/messdaten')

if not os.path.exists(path + '/messdaten/excel'):
    os.mkdir(path + '/messdaten/excel')

if not os.path.exists(path + '/messdaten/csv'):
    os.mkdir(path + '/messdaten/csv')

# Save to csv and xlsx Files

# df.to_excel(path + "/messdaten/excel/" + filename + ".xlsx")
filename = path + "/messdaten/csv/" + filename + ".csv"
if append and os.path.isfile(filename):
    df.to_csv(filename, index=False, mode='a', header=False)
else:
    df.to_csv(filename, index=False)
