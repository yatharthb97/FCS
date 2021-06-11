#!/usr/bin/env python3
# coding: utf-8

#Shapes

import numpy as np
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
import sys
import os

def Ellipsoid(xr, yr, zr):
    N=50
    stride=1
    u = np.linspace(0, 2 * np.pi, N)
    v = np.linspace(0, np.pi, N)
    x = np.outer(np.cos(u), np.sin(v))
    y = np.outer(np.sin(u), np.sin(v))
    z = np.outer(np.ones(np.size(u)), np.cos(v))
    x*=xr
    y*=yr
    z*=zr
    return x,y,z



# Sampling Plot
def Rnd_Sample_Plot(parent_path, show=False):
  filename = os.path.join(parent_path, 'u_dist.dat');
  uniform = np.genfromtxt(filename, unpack = True)

  filename = os.path.join(parent_path, 'gauss_dist.dat');
  gauss = np.genfromtxt(filename,  unpack = True)

  
  # Creating plot → Gaussian Sample
  fig = plt.figure()
  plt.hist(gauss, bins=40)
  plt.title('Random Sample → Gaussian')

  savefigname = os.path.join(parent_path, 'gauss.png')
  plt.savefig(savefigname)

  if show:
    plt.show()


  fig = plt.figure()
  plt.hist(uniform, bins=40)
  plt.title('Random Sample → Uniform')

  savefigname = os.path.join(parent_path, 'u_dist.png')
  plt.savefig(savefigname)

  if show:
    plt.show()


########################## 6.0 3 D Particle Tagging Plot #################
#  /$$$$$$$                       /$$           /$$$$$$$$                 
# | $$__  $$                     | $$          |__  $$__/                 
# | $$  \ $$ /$$$$$$   /$$$$$$  /$$$$$$           | $$  /$$$$$$   /$$$$$$ 
# | $$$$$$$/|____  $$ /$$__  $$|_  $$_/           | $$ |____  $$ /$$__  $$
# | $$____/  /$$$$$$$| $$  \__/  | $$             | $$  /$$$$$$$| $$  \ $$
# | $$      /$$__  $$| $$        | $$ /$$         | $$ /$$__  $$| $$  | $$
# | $$     |  $$$$$$$| $$        |  $$$$/         | $$|  $$$$$$$|  $$$$$$$
# |__/      \_______/|__/         \___/           |__/ \_______/ \____  $$
#                                                                /$$  \ $$
#                                                               |  $$$$$$/
#                                                                \______/ 


#6.1 Particle Tagging Function Defination
def plot_taggedpart(s_size, parent_path, param, show=True):

  filename = os.path.join(parent_path, 'tag.dat');
  x, y, z = np.genfromtxt(filename, delimiter = param['D_Sep'], unpack = True)

  if s_size > np.size(x):
    print(f"[ERROR] Sampling Size is greater than total points → {s_size}")
    s_size = np.size(x) #Set s_size to size of arrays

  color = np.arange(s_size)


  ax3d = plt.subplot(projection='3d')
  ax3d.set_title(f"Tagged Particle PID: {int(param['Tagged Part ID'])} - Steps → 0:{s_size}")

  #Slice x, y, z
  x = x[:s_size]
  y = y[:s_size]
  z = z[:s_size]

  #Range Calculations
  # xm = x.mean() #mean
  # xv = x.var() #Variance
  # xr = xm + (xv/2)

  # ym = y.mean() #mean
  # yv = y.var() #Variance
  # yr = ym + (yv/2)

  # zm = z.mean() #mean
  # zv = z.var() #Variance
  # zr = zm + (zv/2)

  ax3d.set_xlim3d([x.min(), x.max()])
  ax3d.set_ylim3d([y.min(), y.max()])
  ax3d.set_zlim3d([z.min(), z.max()])

  ax3d.set_xlabel('X')
  ax3d.set_ylabel('Y')
  ax3d.set_zlabel('Z')

  ax3d.view_init(25, 10)

  ax3d.plot3D(x, y, z, c = 'grey')
  ax3d.scatter3D(x, y, z, c=color, s=0.2, cmap='viridis')
  #fig.colorbar(color)

  savefigname = os.path.join(parent_path, f"tagged_pid{param['Tagged Part ID']}.png")
  plt.savefig(savefigname)

  if show:
    plt.show()










