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

t, intensity = np.loadtxt("timedata.csv", unpack = True) #Load File
#pd.read_csv('timedata.csv', delimiter = ' ')
print("Unpacked!")

time_step = 0.00001 #1e5
#avg_time = 0.001 #1e3
avg_interval = 10000




#Define autocorrelation function
def autocorr(x):
    result = np.correlate(x, x, mode='full')
    return result[int(result.size/2):]



# start = timeit.timeit()
# print("Timer 1 ")

# autocy1 = autocorr(intensity)
# autocx1 = np.arange(np.size(autoy1))
# plt.plot(autocx1, autocy1, '-',)
# plt.xlabel('k')
# plt.ylabel('G(k)')
# plt.title('Autocorrelation Plot - Full')

# plt.savefig('autocorr1.png')
# plt.show()



#With Averaging
print("Length of Time Signal: {0}".format(t))
avg_size = int(np.size(intensity)/avg_interval)
print("Averaging for every {0} points".format(avg_interval))
#print("Length of time series: {0}".format(avg_size))


print("Timer 2 ")
start = timeit.timeit()

#Calculate array avg
avg = np.zeros(int(avg_size), dtype=float);
#avg = np.zeros(256, dtype=float);
start = 0
end = avg_interval
for i in range(int(avg_size)):
  avg[i] = np.sum(intensity[start:end])
  start = start + avg_interval
  end = end + avg_interval

df=pd.DataFrame(avg, columns=['Val']) 


#autocy = autocorr(avg)
#autocx = np.arange(len(autocy), dtype=float)

y = pd.Series(sm.tsa.acf(avg, nlags=255))
x = np.arange(0, 256)



#np.multiply(autocx, (1/float(avg_interval)))
plt.xscale("log")
plt.plot(x, y, '-',)
plt.xlabel('k')
plt.ylabel('G(k)')
plt.title('Autocorrelation Plot - Averaging')


plt.savefig('autoco.png')

end = timeit.timeit()
print("Elapsed:", end - start)

plt.show()