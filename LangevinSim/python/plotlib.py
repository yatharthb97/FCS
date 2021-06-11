# Plot Module

#!/usr/bin/env python3
# coding: utf-8

#TODO → Clean
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

from shapes import Ellipsoid 


#Grammar → 
#1. #function_name(*datas, *fn_attributes, param, fit)

#Poisson Histogram Plot
def plot_poisson_hist(data, bins, plot_title, param,  exclude_zero=False, fit='fit'):

  fig = plt.figure()
  plt.title(plot_title)

  bin_max = 20
  ag = 1
                     #TODO
  if bins > bin_max: #Just control the ticks and not the bins
    ag = int(bins/bin_max)
    bins = bin_max

  binlist = np.arange(0, bins)
  binlist = binlist*ag
  #print(binlist)
  
  #xtics offset calculation
  offset = np.ones(bins)
  offset = offset*0.5*ag
  offsetbinlist = binlist + offset


  if exclude_zero:
    offsetbinlist = np.delete(offsetbinlist, 0)
    binlist = np.delete(binlist, 0)
    #offsetbinlist = offsetbinlist - 1

  plt.xticks(offsetbinlist, binlist.astype(int))
  #plt.yscale('log')

  entries, bin_edges, patches = plt.hist(data, bins=binlist, align='mid', density=True, rwidth=0.9, histtype='bar')


  if exclude_zero:
    print(f"• (Zero Excluded) Count for zero → {entries[0]}")
    #Pop Topmost Elements
    print(entries)
    entries = np.delete(entries, 0)
    print(entries)
    bin_edges = np.delete(bin_edges, 0)
  
  # calculate bin centres
  bin_centres = (bin_edges[1:] + bin_edges[:-1])


  def poisson_fit(k, lamb):
      '''poisson function, parameter lamb is the fit parameter'''
      return poisson.pmf(k, lamb)


  # fit with curve_fit
  paras, cov = curve_fit(poisson_fit, bin_centres, entries)
  print(f"• Poisson Fit Parameters for: {plot_title}")
  print(f"• k Calculated from poisson-fitting → {paras[0]}")

  #print(f"• lamda Calculated from poisson-fitting → {paras[1]}")
  

  # plot poisson-deviation with fitted parameter
  x = np.linspace(0, binlist[-1], 50)
  #plt.plot(binlist, poisson_fit(x, *paras), marker='o', linestyle='-',label='Fit result')
  lamd = np.mean(data)
  # x_plot = np.arange(0, max(data) + 1)
  plt.plot(x, poisson_fit(x, *paras), '--', label='fit')

  savefigname = os.path.join(param['parent_path'], f'{plot_title}.png')
  plt.savefig(savefigname)

  if param['show_py_plots']:
    plt.show()


#Plot MSD
def plot_msd(t, msd, param, log=False, fit='none'):
  
  #Setup resource
  fig, ax = plt.subplots()

  #Log Axes Toggle
  if log:
    ax.set_xscale('log')
    ax.set_yscale('log')

  ax.plot(t, msd, '-',label = 'data')

  msd_fit_deg = 1
  if fit != 'none': #MSD Polynomial fit
    coef = np.polyfit(t, msd, msd_fit_deg) #Fit polynomial
    msd_fit_fn = np.poly1d(coef)
    
  #DO Fit Plotting
  if(fit == 'plot') and (fit != 'none'):
    #Print Polynomial Fit 
    print(f"\n• MSD Polynomial Fit → degree [{msd_fit_deg}]: {msd_fit_fn}")
    ax.plot(t, msd_fit_fn(t), '--k', label = 'fit')
    
    #Saturation Estimate
    if param['PBC']:
        t_saturate = param['Edge'] * 2/(2 * param['dim'] * param['D'])
        print(f"\n• MSD Saturation Estimate: {t_saturate}")
        
        #arrow(x, y, dx, dy, **kwargs) → dx, dy: The length of the arrow along x & y
        # plt.arrow(t_saturate, 0, 0, msd[int(t_saturate)], 
        #   head_width = 0.2,
        #   width = 0.05,
        #   ec ='green')

    #Plot Data
    ax.set_title(f'MSD Plot - Slope → {coef[0]}') #TODO - Restrict Precision
    ax.set_xlabel('time')
    ax.set_ylabel('MSD')

    plt.legend()

    msdfigname = os.path.join(param['parent_path'], 'msdplot.png')
    plt.savefig(msdfigname)

    # Open Live Plot
    if param['show_py_plots']:
        plt.show()

#Plot ACF
def plot_acf(acf_x, acf_y, param, polyfit = 0, gauss_fit = True, draw_model=True):
  
  #Init Resource
  fig, ax = plt.subplots()

  #Plot Data
  ax.set_title('Autocorrelation Plot')
  ax.plot(acf_x, acf_y, '-', label = 'data')
  #plt.fill_between(acf_x, (acf_y - ci), (acf_y+ci), color='blue', alpha=0.1)
  
  #TauD Estimation
  TauD = (param['veff_radius']**2) / (param['D'] * 6.0)
  print(f"• TauD Calculated from model → {TauD}")
  sf_sq = param['veff_sf']**2

  
  #Fit Functions
  def FitFixedTau(T):
      return 1 / ((1 + (T/TauD))* np.sqrt(1 + (T/TauD)/sf_sq))
  def FitGaussFn(T, a, b):
      return a / ((1 + (T*b))* np.sqrt(1 + (T*b)/sf_sq))

  if draw_model:

    x_model_factor = param['bin_size'] * param['dt'] * acf_y[0]
    ax.plot(acf_x, FitFixedTau(acf_x * x_model_factor), '--r', label = 'model')

  if polyfit > 0:

    #4.4 ACF Curve Fitting
    acf_coef = np.polyfit(acf_x, acf_y, polyfit) #Fit polynomial of degree 1
    acf_fit_fn = np.poly1d(acf_coef) 
    print(f"\n• ACF Polynomial Fit → degree [{polyfit}]: \n{acf_fit_fn}") #Print ACF Fit Polynomial
    ax.plot(acf_x, acf_fit_fn(acf_x), '--k', label = f'poly({polyfit}) fit')


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


  # Save Plot
  acffigname = os.path.join(param['parent_path'], 'acfplot.png')
  plt.savefig(acffigname)

  # Open Live Plot
  if param['show_py_plots'] == True:
    plt.show()


# Time Series Plot
# title → f'Photon Incidence Inensity Plot'
def plot_timeseries(time_series, plot_title, param):
  fig, ax = plt.subplots()

  tx = np.arange(np.size(time_series))
  ax.set_title(plot_title)
  ax.plot(tx, time_series, '-')

  ax.set_xlabel('time(t) →')
  ax.set_ylabel('Count →')

  savefigname = os.path.join(param['parent_path'], 'pcounts.png')
  plt.savefig(savefigname)

  if param['show_py_plots']:
    plt.show()

def plot_two_timeseries(data1, data2, label1, label2, plot_title, param):
  
  fig, ax = plt.subplots()
  tx = np.arange(max(np.size(data1), np.size(data2)))
  
  ax.set_title(plot_title)
  ax.plot(tx, data1, '-', label = label1)
  ax.plot(tx, data2, '-', label = label2)
  
  
  y_max = max(data1.max(), data2.max())


  plt.legend()
  ax.set_yticks(np.arange(0, y_max))
  ax.set_xlabel('time →')
  ax.set_ylabel('Count →')
  
  #3.4 Save Plot
  figname = os.path.join(param['parent_path'], f'{plot_title}.png')
  plt.savefig(figname)

  # 3.5 Open Live Plot
  if param['show_py_plots'] == True:
    plt.show()


#6.1 Particle Tagging Function Defination
def plot_taggedpart(sample_size, param):

  filename = os.path.join(param['parent_path'], 'tag.dat');
  x, y, z = np.genfromtxt(filename, delimiter = param['D_Sep'], unpack = True, 
                          max_rows=sample_size)

  if sample_size > np.size(x):
    sample_size = np.size(x)
    print(f"[ERROR] Sample Size Shortage. Actual Size → {sample_size}")

  color = np.arange(sample_size)

  ax3d = plt.subplot(projection='3d')
  ax3d.set_title(f"Tagged Particle PID: {int(param['Tagged Part ID'])} - Steps → 0:{sample_size}")

  #Adjust Limits Based on min and max position Components
  ax3d.set_xlim3d([x.min(), x.max()])
  ax3d.set_ylim3d([y.min(), y.max()])
  ax3d.set_zlim3d([z.min(), z.max()])

  ax3d.set_xlabel('X →')
  ax3d.set_ylabel('Y →')
  ax3d.set_zlabel('Z →')

  ax3d.view_init(25, 10)

  #Order of magnitude scaling for Point Size : 10 → 0.4 100 → 0.3, 1000 point → 0.2
  point_size = (math.log10(sample_size) - 1.0)/10
  print(point_size)

  ax3d.plot3D(x, y, z, c = 'grey') #Path

  ax3d.scatter3D(x, y, z, c=color, s=point_size, cmap='viridis') #Point Cluster
  #fig.colorbar(color)

  savefigname = os.path.join(parent_path, f"tagged_pid{param['Tagged Part ID']}.png")
  plt.savefig(savefigname)

  if show:
    plt.show()


# Sampling Plot
def plot_rndsample_hist(bins, param):
  filename = os.path.join(param['parent_path'], 'u_dist.dat');
  uniform = np.genfromtxt(filename, unpack = True)

  filename = os.path.join(parent_path, 'gauss_dist.dat');
  gauss = np.genfromtxt(filename,  unpack = True)

  
  # Creating plot → Gaussian Sample
  fig = plt.figure()
  plt.hist(gauss, bins=bins)
  plt.title('Random Sample → Gaussian')

  savefigname = os.path.join(parent_path, 'gauss.png')
  plt.savefig(savefigname)

  if param['show_py_plots']:
    plt.show()


  fig = plt.figure()
  plt.hist(uniform, bins=bins)
  plt.title('Random Sample → Uniform')

  savefigname = os.path.join(parent_path, 'u_dist.png')
  plt.savefig(savefigname)

  if param['show_py_plots']:
    plt.show()

#Plot Position Animation
def pos_plot_animation(param, save_vid=False, psf_alpha=0.25, draw_psf=True):

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
      filename = os.path.join(param['parent_path'], file);
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
    if param['show_py_plots'] == True:
      plt.show()
   
    print("••• Plot animation ends...")
    #---

    #5.9 Save the animation
    if save_vid:
      print("••••• Rendering Plot animation begins...")
      vidname = os.path.join(param['parent_path'], 'posplots.mp4')
      ani.save(vidname)
      print("••••• Rendering Plot animation ends...")



def bin_tseries(t_series, bin_size):
  binned_ts = np.copy(t_series) #Make a Copy of flash
  binned_ts = np.reshape(binned_ts, newshape=(int(np.size(t_series)/bin_size), bin_size))
  binned_ts = np.sum(binned_ts, axis=1) #Sum along the row
  return binned_ts