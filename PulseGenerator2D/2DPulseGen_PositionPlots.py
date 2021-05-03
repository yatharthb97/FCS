#!/usr/bin/env python3
# coding: utf-8


#1. Import Block ########################################################

import sys
import os

#Serial Lib Check -------------------------------------------------------
try:
  import serial.tools.list_ports as port_list
except ImportError:
  print ("Import Error - serial")
  import subprocess

  subprocess.check_call([sys.executable, "-m", "pip", "install", "pyserial"])
  import serial.tools.list_ports as port_list

from serial import Serial
#--------------------------------------------------------------------------
#
#
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
#--------------------------------------------------------------------------
#
# Inlining 
#get_ipython().run_line_magic('matplotlib', 'notebook')
print("Import Complete")


##############################################################################



#2. Global Variables ###################################
D_Sep = ','
session_name = "Test"


#Receive environemt from args
#ARG0 - Scriptname
#ARG1 - ParentPath
#ARG2 - EDGE
#ARG3 - totalrames
#ARG4 - OBx.min
#ARG5 - OBx.max
#ARG6 - OBy.min
#ARG7 - OBz.max

parent_path = sys.argv[1]
edge = int(sys.argv[2])
frames = int(sys.argv[3])

#Boundary Values
OBxmin = int(sys.argv[4])
OBxmax = int(sys.argv[5])
OBymin = int(sys.argv[6])
OBymax = int(sys.argv[7])



xe = []
ye = []
#Create 2d Histogram
fig, ax = plt.subplots(figsize = (8,8))
data,x,y = np.histogram2d(xe,ye)
im = plt.imshow(data.T, interpolation = False, origin = 'lower')


#5. SerialRead Function ########################

def graph_update(i):       
  #Read from the s_port

  file = str(i).append('.dat')
  filename = os.path.join(parent_path, file);

  readflag = false;
  while(!readflag):
      if (!os.path.exists(filename)
        sleep(0.01)
      else
        xx, yy  = np.loadtxt(filename, delimator = ',', unpack = True)# Get Your Data From txt
        data, x, y = np.histogram2d(xx, yy)
        im.set_data(data)
        # ax.clear()
        # ax.plot(x,y)
        # ax.hist2d(x, y)
        readlag = True
        #print(serialarray)
#End of SerialRead()

#########################################################




#8. Animation Section



print("Read begins...")
#--
ani = animation.FuncAnimation(fig, animate, np.arange(0,frames),
                          interval = 1, blit = False) 

plt.show()
#---
print("Read ends...")


#Save the last snapshot
plt.savefig(session_name.join('.png'), bbox_inches='tight')
#pylab leaves a generous, often undesirable, whitespace around the image → 'tight'

#Save the animation
#ani.save(session_name.join('.mp4'),writer=writer)







