# -*- coding: utf-8 -*-
"""
Created on Thu May 13 19:03:31 2021

@author: yilei
"""

# reset -f
# clear

# after the second sound of the connection, start this programm!
# read arduino from serial and make a plot
import serial
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import os

n = 1
filename = 'test'

# sets up serial connection (make sure baud rate is correct - matches Arduino)
# before starting connection  Arduino with IDE, make sure this serical stay closed. Otherwise this connection can not be done.
# the error from Arduino is: Executing command: exit status 1

try:
    ser = serial.Serial('COM4', 9600)
    ser.close()
except OSError:
    print("The nummber of serical maybe not right or the conncetion is not builded. Serical is ready for reading！\n")

ser = serial.Serial('COM4', 9600)
ser.timeout = 2  # waiting time  for reading

a = []  # a for saving data

i = 1
while i <= n:
    data = ser.readline()  # read the serial
    data = data.decode()  # the read date is in form bytes，so it need to be changed in string
    data = data.split(",")
    if len(data) < 1:  # when no more data come, the loop will be stopped.
        break
    elif len(data) == 1:
        print("No more data from Ardunio...")
        break
    else:
        data = list(map(float, data))  # 把字符串转化为数字-->[180.87, 2.16, -3.86]
    print(data)
    a.append(data)  # 添加到列表里

    i += 1

df = pd.DataFrame(
    a,
    columns=['Temp', 'AverageTemp', 'Time', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'R', 'I', 'S', 'J', 'T', 'U', 'V',
             'W', 'K', 'L'])

# Visualisierung mittelwert bilden

plt.plot(df.mean()[3:])
plt.title('Temp: ' + str(round(df.mean()[1], 1)))

ser.close()

print(a)
path = os.getcwd()
if not os.path.exists(path + '/messdaten'):
    os.mkdir(path + '/messdaten')

if not os.path.exists(path + '/messdaten/excel'):
    os.mkdir(path + '/messdaten/excel')

if not os.path.exists(path + '/messdaten/csv'):
    os.mkdir(path + '/messdaten/csv')

df.to_excel(path + "/messdaten/excel/" + filename + ".xlsx")
df.to_csv(path + "/messdaten/csv/" + filename + ".csv", index=False)
