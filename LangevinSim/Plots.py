#!/usr/bin/env python3
# coding: utf-8



#TODO
# • Add Legends and Labels to graphs
# • ACF Fit Block
# •

#1. Import Block ########################################################

# This block makes sure that the packages that are not present
# are installed during the first ever execution of the program.
# For Actual Code → Line No :  TODO

#1.1 Must Import
import sys
import os


#matplotlib Lib Check -------------------------------------------------------
try:
  import matplotlib.pyplot as plt
except ImportError:
  print ("Import Error - matplotlib")
  import subprocess

  subprocess.check_call([sys.executable, "-m", "pip", "install", "matplotlib"])
  import matplotlib.pyplot as plt
#--------------------------------------------------------------------------

#numpy Lib Check -------------------------------------------------------
try:
  import numpy as np
except ImportError:
  print ("Import Error - matplotlib")
  import subprocess

  subprocess.check_call([sys.executable, "-m", "pip", "install", "numpy"])
  import numpy as np
#--------------------------------------------------------------------------
#
#
#import animation ---------------------------------------------------------
from matplotlib import animation
from mpl_toolkits import mplot3d
plt.style.use('seaborn-darkgrid')

#TODO -> Check for these libraries too
import statsmodels.api as sm
#from scipy import curve_fit
import math
import json
import datetime
#--------------------------------------------------------------------------



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


#2.3 Misc Parameter Declarations

do_MSD_Fit = True
do_ACF_Fit = True
msd_fit_deg = 1
acf_fit_deg = 1

bin_size = int(100)
lags = 256
param['Edge'] = param['Edge']/2 #TODO Specifiy why it is necessary
###################################

#--- Setup is complete.


#2. Load stats file
statfilename = os.path.join(parent_path, 'stats.dat');

t, msd, invol, og_flash = np.loadtxt(statfilename, delimiter = param['D_Sep'], unpack = True)

flash = invol #Set flash as invol TODO !!!

# --------------  3.0 -------------
# /$$      /$$  /$$$$$$  /$$$$$$$ 
#| $$$    /$$$ /$$__  $$| $$__  $$
#| $$$$  /$$$$| $$  \__/| $$  \ $$
#| $$ $$/$$ $$|  $$$$$$ | $$  | $$
#| $$  $$$| $$ \____  $$| $$  | $$
#| $$\  $ | $$ /$$  \ $$| $$  | $$
#| $$ \/  | $$|  $$$$$$/| $$$$$$$/
#|__/     |__/ \______/ |_______/ 


#3.1 MSD Polynomial fit
coef = np.polyfit(t, msd, msd_fit_deg) #Fit polynomial of degree 1
msd_fit_fn = np.poly1d(coef) 
# msd_fit_fn is now a function which takes in x and returns an estimate for y

#3.2 Print Polynomial Fit 
print(f"\n• MSD Polynomial Fit: {msd_fit_fn}")

#3.3 Plot MSD Graph ------------------------------------
if do_MSD_Fit == True:
  plt.title(f'MSD Plot - Slope → {coef[0]}')
  plt.plot(t, msd, '-',label = 'data')
  plt.plot(t, msd_fit_fn(t), '--k', label = 'fit')
  plt.legend()

else: #Do not add Fitting curve
  print("-> MSD Fitting is disabled.")
  plt.title('MSD Plot')
  plt.plot(t, msd, '-', label = 'data')
  plt.legend()
   
# naming the x axis
plt.xlabel('time')
# naming the y axis
plt.ylabel('MSD')

#3.4 Save Plot
msdfigname = os.path.join(parent_path, 'msdplot.png')
plt.savefig(msdfigname)

# 3.5 Open Live Plot
if param['show_py_plots'] == True:
  plt.show()
# ---------------------------------------------------

## ------------  4.0 ---------
#  /$$$$$$   /$$$$$$  /$$$$$$$$
# /$$__  $$ /$$__  $$| $$_____/
#| $$  \ $$| $$  \__/| $$      
#| $$$$$$$$| $$      | $$$$$   
#| $$__  $$| $$      | $$__/   
#| $$  | $$| $$    $$| $$      
#| $$  | $$|  $$$$$$/| $$      
#|__/  |__/ \______/ |__/      
                              
                              
                              
#4.1 Calculation of Array Sizes Average out flash bool values
tseries_size = np.size(flash)
binned_ts_size = int(tseries_size/bin_size)

# 4.2 Print Essential Statistics
stat_txt = f"""
• Length of Time Series: {tseries_size}
• Number of flashes(total): {int(np.sum(flash))}
• Number of invols(total): {int(np.sum(invol))}
• Bin Size: {bin_size}
• Length of Binned Time Series: {int(binned_ts_size)}"""
print(stat_txt)



#4.3 Time Series Binning
binned_ts = np.copy(flash) #Make a Copy of flash
binned_ts = np.reshape(binned_ts, newshape=(binned_ts_size, bin_size))
binned_ts = np.sum(binned_ts, axis=1) #Sum along the row



#4.3 Calculation of ACF
acf_y = sm.tsa.acf(binned_ts, nlags=lags, fft=False)
acf_x = np.arange(lags+1)


#4.4 ACF Curve Fitting
acf_coef = np.polyfit(acf_x, acf_y, acf_fit_deg) #Fit polynomial of degree 1
acf_fit_fn = np.poly1d(acf_coef) 
print(f"\n• ACF Polynomial Fit: \n{acf_fit_fn}") #Print ACF Fit Polynomial

#popt, pcov = curve_fit(GaussianFit, xdata, ydata)

#4.5 Plot MSD Graph ------------------------------------

plt.xscale("log")
plt.xlabel(r'$log_{e}(k)$ → lag')
plt.ylabel(r'$G(k)$ → ACF')

if do_ACF_Fit == True:
  plt.title('Autocorrelation Plot')
  plt.plot(acf_x, acf_y, '-', label = 'data')
  plt.plot(acf_x, acf_fit_fn(acf_x), '--k', label = 'fit')
  plt.legend()

else: #Do not add Fitting curve
  print("-> ACF Fitting is disabled.")
  plt.title('Autocorrelation Plot')
  plt.plot(acf_x, acf_y, '-', label = 'data')
  plt.legend()

#4.6 Save ACF Plot
figname = os.path.join(parent_path, 'autocorr.png')
plt.savefig(figname)

#4.7 Open Live Plot
if param['show_py_plots'] == True:
  plt.show()



########################## 5.0 3 D Position Plots ################################

# /$$$$$$$                           /$$$$$$$  /$$             /$$    
#| $$__  $$                         | $$__  $$| $$            | $$    
#| $$  \ $$ /$$$$$$   /$$$$$$$      | $$  \ $$| $$  /$$$$$$  /$$$$$$  
#| $$$$$$$//$$__  $$ /$$_____/      | $$$$$$$/| $$ /$$__  $$|_  $$_/  
#| $$____/| $$  \ $$|  $$$$$$       | $$____/ | $$| $$  \ $$  | $$    
#| $$     | $$  | $$ \____  $$      | $$      | $$| $$  | $$  | $$ /$$
#| $$     |  $$$$$$/ /$$$$$$$/      | $$      | $$|  $$$$$$/  |  $$$$/
#|__/      \______/ |_______/       |__/      |__/ \______/    \___/  

#Enabled by a toggle                                                             
if param['do_pos_plots'] == True:

    #5.0 Set Style to default
    plt.rcParams.update(plt.rcParamsDefault)


    #5.1 Declare Arrays
    xdata = np.empty(param['Part_no'])
    ydata = np.empty(param['Part_no'])
    zdata = np.empty(param['Part_no'])
    cdata = np.empty(param['Part_no'])
    isinvol = np.empty(param['Part_no'])
    isflash = np.empty(param['Part_no'])

    #5.2 Create 3D Plot - init
    fig_pos = plt.figure()
    ax3d = plt.axes(projection='3d')
    ax3d.scatter3D(xdata, ydata, zdata, c=cdata);


    #5.3 Setting the axes properties
    ax3d.set_xlim3d([-param['Edge'], param['Edge']])
    ax3d.set_xlabel('X')

    ax3d.set_ylim3d([-param['Edge'], param['Edge']])
    ax3d.set_ylabel('Y')

    ax3d.set_zlim3d([-param['Edge'], param['Edge']])
    ax3d.set_zlabel('Z')

    #5.4 Set Title
    ax3d.set_title('Position Plots')

    #5.5 Provide starting angle for the view.
    ax3d.view_init(25, 10)


    #5.6 graph_update function ########################
    def graph_update(i):
      # Get Data from file
      file = str(i)
      file += ".dat"
      filename = os.path.join(parent_path, file);
      xdata, ydata, zdata, isinvol, isflash  = np.genfromtxt(filename, delimiter = ",", unpack = True)

      cdata = isinvol + isflash #Set Colordata

      #TODO
      colordict = {
                     0: "purple",
                     1: "Mustang",
                     2: "yellow"
                  }

      ax3d.clear()
      ax3d.set_title(f'Position Plot → {str(i)}')
      
      ax3d.set_xlim3d([-param['Edge'], param['Edge']])
      ax3d.set_xlabel('X')

      ax3d.set_ylim3d([-param['Edge'], param['Edge']])
      ax3d.set_ylabel('Y')

      ax3d.set_zlim3d([-param['Edge'], param['Edge']])
      ax3d.set_zlabel('Z')
      
      ax3d.scatter3D(xdata, ydata, zdata, c=cdata);
    ##################################### End of graph_update()


    #5.7. Animation Section
    #-- TODO if show_plot is false, do a quick render ? (This takes 0.5*(frames+1)*2 seconds)
    print("••• Plot animation begins...")
    ani = animation.FuncAnimation(fig_pos, graph_update, frames = np.arange(0, param['FrameExports']+1),
                                  blit = False, repeat = False, interval = 500) 
  
    #5.7.1 → Show animation plots
    if param['show_py_plots'] == True:
      plt.show()
   
    print("••• Plot animation ends...")
    #---

    #5.8 Save the last snapshot
    lastplotname = os.path.join(parent_path, 'last_plot.png')
    plt.savefig(lastplotname)

    #5.9 Save the animation
    vidname = os.path.join(parent_path, 'posplots.mp4')
    ani.save(vidname)

else:
  print(f"-> Positon Plots are disabled.")

#6.0 Print Exit Message
now = datetime.datetime.now()
dt_str = now.strftime("%d-%m-%Y %H:%M:%S")
print(f"Script exited successfully: {dt_str}")

