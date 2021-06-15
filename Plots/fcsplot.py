#!/usr/bin/env python3
# coding: utf-8



import sys
import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import animation
from mpl_toolkits import mplot3d
import timeit
import statsmodels.api as sm
import pandas as pd

x, y = np.loadtxt("FCSreal2.dat", unpack = True) #Load File
#pd.read_csv('timedata.csv', delimiter = ' ')
print("Unpacked!")


rx = np.arange(np.size(x))
plt.plot(rx, x, '-',)
plt.xlabel('series length')
plt.ylabel('val')
plt.title('Value of time tics in FCS data')



# xs = np.arange(np.size(y))
# plt.xscale("log")
# plt.plot(xs, y, '-',)
# plt.xlabel('k')
# plt.ylabel('G(k)')
# plt.title('Autocorrelation Plot - direct')


plt.savefig('autoco.png')

plt.show()