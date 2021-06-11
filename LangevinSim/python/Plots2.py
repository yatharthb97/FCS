#!/usr/bin/env python3
# coding: utf-8



#Schematics

#langplot lib
  #Shapes
  #Plot Funcs
  #plot_script

#Grammar
  #methods → all lowercase
  #function_name(*datas, *fn_attributes, param, fit)


#1. Import Block ########################################################

# This block makes sure that the packages that are not present
# are installed during the first ever execution of the program.
# For Actual Code → Line No :  TODO

#1.1 Imports
import sys
import os
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import LogLocator
from matplotlib import animation
from mpl_toolkits import mplot3d

import statsmodels.api as sm
from scipy.optimize import curve_fit

from scipy.stats import poisson
import math
import json
import datetime
from shapes import *
from plotlib import *
#Contains the 3D Ellipsoid Generator

#Declare Plot Style
plt.style.use('seaborn-darkgrid')

#2. Parameter Setting ###################################


#2.1 Receive environemt from args
#ARG0 - Scriptname
parent_path = sys.argv[1] #ParentPath

# Set if the script is called independently or by C++ Simulator → "C++"
if len(sys.argv) > 2:
  operation_mode = sys.argv[2]
else:
  operation_mode = "Manual"

#Print Parent Path and Operation Mode
print(f"• Parent Path: {parent_path} | Operation Mode: {operation_mode}");


#2.2 Open JSON config file
jsonname = os.path.join(parent_path, 'config.json')
with open(jsonname) as json_file:
    param = json.load(json_file)
#print(param)

#2.3 Check for Symmetric/Asymmetric mode
if param['Symmetric Box']:
    param['Edge'] = param['Edge']/2 #TODO Specifiy why it is necessary
param['parent_path'] = parent_path;

#2.4 Misc Parameter Declarations
do_MSD_Fit = False
do_ACF_Fit = True
do_gauss_fit = True
draw_gauss_model= True

msd_log_scale = False
msd_fit_deg = 1
acf_fit_deg = 3

param['bin_size'] = int(100) #For TimeSeries
lags = 256
psf_alpha = 0.15
tag_sample_size = 1000
#--- Setup is complete.


#2.0 Load stats file
statfilename = os.path.join(parent_path, 'stats.dat');
t, msd, invol, flash = np.loadtxt(statfilename, delimiter = param['D_Sep'], unpack = True)



#Plot Dual Time Series of whole arrays
plot_two_timeseries(invol, flash, "invol", "flash", "Count Plot", param)


#PoissonPlot(data, bins, plot_title, param,  fit)
plot_poisson_hist(flash, 10, "Flash Poisson", param, False)
plot_poisson_hist(invol, 10, "InVol Poisson", param, False)

# --------------  3.0 -------------

#plot_msd(t, msd, log, fit=True)
plot_msd(t, msd, param, log=False, fit='plot')

## ------------  4.0 ---------  
                              
#4.1 Calculation of Array Sizes Average out flash bool values
tseries_size = np.size(flash)
binned_ts_size = int(tseries_size/param['bin_size'])

# 4.2 Print Essential Statistics
stat_txt = f"""
• Length of Time Series: {tseries_size}
• Number of flashes(total): {int(np.sum(flash))}
• Number of invols(total): {int(np.sum(invol))}
• Bin Size: {param['bin_size']}
• Length of Binned Time Series: {int(tseries_size/param['bin_size'])}"""
print(stat_txt)



#4.3 Time Series Binning
#bin_tseries(t_series, bin_size)
binned_ts = bin_tseries(flash, param['bin_size'])

# Call Plot PCOUNTS
#plot_timeseries(time_series, plot_title, param)
plot_timeseries(binned_ts, "Averaged Time Series", param)

poisson_bin_size = 200
poisson_binned_ts = bin_tseries(flash, poisson_bin_size)

plot_poisson_hist(poisson_binned_ts, 40, f"Bin Size {poisson_bin_size} - Poisson", param, exclude_zero=False, fit=True)
#Poisoon Histogram of Averged Time Series
#plot_poisson_hist(data, bins, plot_title, param, fit):



#4.3 Calculation of ACF
acf_y = sm.tsa.acf(binned_ts, nlags=lags, fft=False, adjusted=True) #default → bartlett_confint=True
acf_x = np.arange(lags+1)


print(f"• Mean of flashes (data)→  {flash.mean()}")
print(f"• Mean of flashes (model - ACF)→  {1/acf_y[0]}")
print(f"• Molecular Brightness →  {binned_ts.mean()/(param['bin_size']*param['Part_no'])}")
#The molecular brightness is the average numbr of detected photons per sampling time per molecule.


# Call ACF Plot Function
#plot_acf(acf_x, acf_y, param, polyfit = False, gauss_fit = True, draw_model=True)
plot_acf(acf_x, acf_y, param, polyfit = acf_fit_deg, gauss_fit = True, draw_model=True)

########################## 5.0 3 D Position Plots ####################


#5.0.1 Pos Plot Animation Function Enabled by a toggle
if param['do_pos_plots'] == True:
  pos_plot_animation(param,save_vid=False, psf_alpha=psf_alpha, draw_psf=True)

else:
  print(f"-> Positon Plots are disabled.")



# PARTICLE TAGGING
#6.2 Particle Tagging Function Call
if param['Particle Tagging']:
  plot_taggedpart(tag_sample_size, param)



#RANDOM SAMPLING
if param['Rnd Sampling']:
  plot_rndsample_hist(100, param)


#8.0 Print Exit Message
now = datetime.datetime.now()
dt_str = now.strftime("%d-%m-%Y %H:%M:%S")
print(f"Script exited successfully: {dt_str}")