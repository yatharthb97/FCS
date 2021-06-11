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

bin_size = int(100)
lags = 256
psf_alpha = 0.15
tag_particle_size = 1000
#--- Setup is complete.




#2.0 Load stats file
statfilename = os.path.join(parent_path, 'stats.dat');
t, msd, invol, flash = np.loadtxt(statfilename, delimiter = param['D_Sep'], unpack = True)

#2.1 Plot InVol and Flash Function
def plot_flash_invol(flashx, involx, parent_pathx, show = False):
  fig, ax = plt.subplots()
  tx = np.arange(np.size(involx))
  ax.set_title(f'InVol and Flashes')
  ax.plot(tx, involx, '-', label = 'in vol')
  ax.plot(tx, flashx, '-', label = 'flash')
  
  
  plt.legend()
  ax.set_yticks([0,1,2,3])
  ax.set_xlabel('time')
  ax.set_ylabel('Count (N)')
  
  #3.4 Save Plot
  figname = os.path.join(parent_pathx, 'count.png')
  plt.savefig(figname)

  # 3.5 Open Live Plot
  if show == True:
    plt.show()


#PoissonPlot(data, bins, plot_title, param,  fit)
PoissonHistPlot(flash, 10, "Flash Poisson", param, True)
PoissonHistPlot(invol, 10, "InVol Poisson", param, True)


#2.2 Call InVol and Flash Function
plot_flash_invol(flash, invol, parent_path, show = param['show_py_plots'])

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
coef = np.polyfit(t, msd, msd_fit_deg) #Fit polynomial
msd_fit_fn = np.poly1d(coef) 
# msd_fit_fn is now a function which takes in x and returns an estimate for y

#3.2 Print Polynomial Fit 
print(f"\n• MSD Polynomial Fit → degree [{msd_fit_deg}]: {msd_fit_fn}")

#3.3 Plot MSD Function Defination  ------------------------------------
def plot_msd(t, msd, parent_path, show = False, fit=True):
  fig, ax = plt.subplots()

  ax.set_title(f'MSD Plot - Slope → {coef[0]}')
  ax.plot(t, msd, '-',label = 'data')
  
  if fit:
     ax.plot(t, msd_fit_fn(t), '--k', label = 'fit')
     t_saturate = param['Edge'] * 2/(2 * param['dim'] * param['D'])
     print(f"\n• MSD Saturation Estimate: {t_saturate}")
     plt.arrow(t_saturate*param['dt'], 0, 0, msd[int(t_saturate)], ec='red')

  plt.legend()

  ax.set_xlabel('time')
  ax.set_ylabel('MSD')

  if msd_log_scale:
    ax.set_xscale('log')
    ax.set_yscale('log')

  #3.4 Save Plot
  msdfigname = os.path.join(parent_path, 'msdplot.png')
  plt.savefig(msdfigname)

  # 3.5 Open Live Plot
  if show == True:
    plt.show()

#3.4 Plot MSD function called
plot_msd(t, msd, parent_path, show = param["show_py_plots"], fit=do_MSD_Fit)
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


# Call Plot PCOUNTS
PlotPCounts(binned_ts, parent_path, param['show_py_plots'])
#PoissonHistPlot(data, bins, plot_title, param, fit):
PoissonHistPlot(binned_ts, 100, "Avg Poisson", param, True)


#4.3 Calculation of ACF
acf_y = sm.tsa.acf(binned_ts, nlags=lags, fft=False)
acf_x = np.arange(lags+1)


#4.4 ACF Curve Fitting
acf_coef = np.polyfit(acf_x, acf_y, acf_fit_deg) #Fit polynomial of degree 1
acf_fit_fn = np.poly1d(acf_coef) 
print(f"\n• ACF Polynomial Fit → degree [{acf_fit_deg}]: \n{acf_fit_fn}") #Print ACF Fit Polynomial

#popt, pcov = curve_fit(GaussianFit, xdata, ydata)



#4.5 Plot ACF Function defination ------------------------------------
def plot_acf(acf_x, acf_y, polyfit=False, gauss_fit = True, draw_model=True, show = False):
  fig, ax = plt.subplots()

  ax.set_title('Autocorrelation Plot')
  ax.plot(acf_x, acf_y, '-', label = 'data')
  

  TauD = (param['veff_radius']**2) / (param['D'] * 6.0)
  print(f"• TauD Calculated from model → {TauD}")
  sf_sq = param['veff_sf']**2

  def FitFixedTau(T):
      return 1 / ((1 + (T/TauD))* np.sqrt(1 + (T/TauD)/sf_sq))
  def FitGaussFn(T, a, b):
      return a / ((1 + (T*b))* np.sqrt(1 + (T*b)/sf_sq))

  if draw_model:

    x_model_factor = bin_size * param['dt'] * acf_y[0]
    ax.plot(acf_x, FitFixedTau(acf_x * x_model_factor), '--r', label = 'model')

  if polyfit:
    ax.plot(acf_x, acf_fit_fn(acf_x), '--k', label = f'poly({acf_fit_deg}) fit')
  
  if gauss_fit:
    pars, cov = curve_fit(f=FitGaussFn, xdata=acf_x, ydata=acf_y, p0=[0, 0], bounds=(-np.inf, np.inf))
    print(f"• TauD Calculated from curve-fitting → {pars[1]}")
    print(f"• Fit Parameters: {pars}")
    #print(f"• Fit Covariance: {cov}")
    
    print(f"• Scale factor Calculated from curve-fitting → {pars[0]}")
    #b must be inverted to get TauD
    ax.plot(acf_x, FitGaussFn(acf_x, *pars), '--g', label = 'gauss fit')


  plt.legend()
  ax.set_xscale('log')
  ax.xaxis.set_major_locator(LogLocator(base=10.0)) #Set appropriate tics
  ax.set_xlabel(r'$log_{10}(k)$ → $log_{10}(lag)$')
  ax.set_ylabel(r'$G(k)$ → ACF')


  #3.4 Save Plot
  acffigname = os.path.join(parent_path, 'acfplot.png')
  plt.savefig(acffigname)

  # 3.5 Open Live Plot
  if show == True:
    plt.show()

#4.6 Call ACF Plot Function
plot_acf(acf_x, acf_y, polyfit=True, gauss_fit = do_gauss_fit, draw_model=draw_gauss_model, show = param['show_py_plots'])


########################## 5.0 3 D Position Plots ####################

# /$$$$$$$                           /$$$$$$$  /$$             /$$    
#| $$__  $$                         | $$__  $$| $$            | $$    
#| $$  \ $$ /$$$$$$   /$$$$$$$      | $$  \ $$| $$  /$$$$$$  /$$$$$$  
#| $$$$$$$//$$__  $$ /$$_____/      | $$$$$$$/| $$ /$$__  $$|_  $$_/  
#| $$____/| $$  \ $$|  $$$$$$       | $$____/ | $$| $$  \ $$  | $$    
#| $$     | $$  | $$ \____  $$      | $$      | $$| $$  | $$  | $$ /$$
#| $$     |  $$$$$$/ /$$$$$$$/      | $$      | $$|  $$$$$$/  |  $$$$/
#|__/      \______/ |_______/       |__/      |__/ \______/    \___/  




#5.0.0 Pos Plot Animation Function Defined
def pos_plot_animation(draw_psf=True, save_vid=True, show=False):

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

    #5.3 Setting the axes
    if param['Symmetric Box']:
      ax3d.set_xlim3d([-param['Edge'], param['Edge']])
      ax3d.set_ylim3d([-param['Edge'], param['Edge']])
      ax3d.set_zlim3d([-param['Edge'], param['Edge']])

    else:
      ax3d.set_xlim3d([0, param['Edge']])
      ax3d.set_ylim3d([0, param['Edge']])
      ax3d.set_zlim3d([0, param['Edge']])

    #5.4 Set Labels
    ax3d.set_xlabel('X')
    ax3d.set_ylabel('Y')
    ax3d.set_zlabel('Z')
    ax3d.set_title('Position Plots')

    #5.5 Provide starting angle for the view.
    ax3d.view_init(25, 10)

    #5.6 Get PSF Values !!!TODO → Only Symmetric Box Implementation
    xg, yg, zg = Ellipsoid(param['veff_radius'], param['veff_radius'], param['veff_radius']*param['veff_sf'])

    #5.6 graph_update function ########################
    def graph_update(i):
      # Get Data from file
      file = str(i)
      file += ".dat"
      filename = os.path.join(parent_path, file);
      xdata, ydata, zdata, isinvol, isflash  = np.genfromtxt(filename, delimiter = param['D_Sep'], unpack = True)

      cdata = isinvol + isflash #Set Colordata

      #TODO
      colordict = {
                     0: "purple",
                     1: "Mustang",
                     2: "yellow"
                  }

      ax3d.clear()
      ax3d.set_title(f'Position Plot → {str(i)}')
      #TODO → Reolve for asymmetric box
      ax3d.set_xlim3d([-param['Edge'], param['Edge']])
      ax3d.set_xlabel('X')

      ax3d.set_ylim3d([-param['Edge'], param['Edge']])
      ax3d.set_ylabel('Y')

      ax3d.set_zlim3d([-param['Edge'], param['Edge']])
      ax3d.set_zlabel('Z')

      if draw_psf:
        ax3d.plot_surface(xg, yg, zg, linewidth=0.0, cstride=1, rstride=1, alpha=psf_alpha, color='yellow')

      ax3d.scatter3D(xdata, ydata, zdata, c=cdata)
    ##################################### End of graph_update()


    #5.7. Animation Section
    #-- TODO if show_plot is false, do a quick render ? (This takes 0.5*(frames+1)*2 seconds)
    print("••• Plot animation begins...")
    ani = animation.FuncAnimation(fig_pos, graph_update, frames = np.arange(0, param['FrameExports']+1),blit = False, repeat = True, interval = 100) 
  
    #5.7.1 → Show animation plots
    if show == True:
      plt.show()
   
    print("••• Plot animation ends...")
    #---

    #5.8 Save the last snapshot
    lastplotname = os.path.join(parent_path, 'last_plot.png')
    plt.savefig(lastplotname)

    #5.9 Save the animation
    if save_vid:
      print("••••• Rendering Plot animation begins...")
      vidname = os.path.join(parent_path, 'posplots.mp4')
      ani.save(vidname)
      print("••••• Rendering Plot animation ends...")

#5.0.1 Pos Plot Animation Function Enabled by a toggle
if param['do_pos_plots'] == True:
  pos_plot_animation(True, False, param['show_py_plots'])

else:
  print(f"-> Positon Plots are disabled.")



# PARTICLE TAGGING
#6.2 Particle Tagging Function Call
if param['Particle Tagging']:
  Particle_Tagging(tag_particle_size, parent_path, param, param['show_py_plots'])



#RANDOM SAMPLING
if param['Rnd Sampling']:
  Rnd_Sample_Plot(parent_path, param['show_py_plots'])




#8.0 Print Exit Message
now = datetime.datetime.now()
dt_str = now.strftime("%d-%m-%Y %H:%M:%S")
print(f"Script exited successfully: {dt_str}")