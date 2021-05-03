#!/usr/bin/env python3
# coding: utf-8


#1. Import Block ########################################################

import sys
import os
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
from mpl_toolkits import mplot3d
#--------------------------------------------------------------------------
#
print("Import Complete")


##############################################################################



#2. Global Variables ###################################
D_Sep = ','


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
edge = float(sys.argv[2])
frames = int(sys.argv[3])
ob_radius = float(sys.argv[4])
part_nums = int(sys.argv[5])


#Print First Plot
statfile = 'stats.dat'
statfilename = os.path.join(parent_path, statfile);

t, msd = np.loadtxt(statfilename, delimiter = ',', unpack = True)

# plotting the points 
plt.plot(t, msd)
   
# naming the x axis
plt.xlabel('time')
# naming the y axis
plt.ylabel('MSD')
  
# giving a title to my graph
plt.title('MSD Plot')

figname = os.path.join(parent_path, 'plot.png')
plt.savefig(figname)


plt.show()
# function to show the plot





########################## 3 D Position Plots ################################3



#Declare Arrays
xdata = np.empty(part_nums)
ydata = np.empty(part_nums)
zdata = np.empty(part_nums)
cdata = np.empty(part_nums)

#Create 3D Plot
fig2 = plt.figure()
#ax = p3.Axes3D(fig)
ax2 = plt.axes(projection='3d')
ax2.scatter3D(xdata, ydata, zdata, c=cdata);


# Setting the axes properties
ax2.set_xlim3d([-edge, edge])
ax2.set_xlabel('X')

ax2.set_ylim3d([-edge, edge])
ax2.set_ylabel('Y')

ax2.set_zlim3d([-edge, edge])
ax2.set_zlabel('Z')

ax2.set_title('Position Plots')

# Provide starting angle for the view.
ax2.view_init(25, 10)





#5. SerialRead Function ########################

def graph_update(i):       
  #Read from the s_port

  file = str(i)
  file += ".dat"

  filename = os.path.join(parent_path, file);
  #print(filename)
  #readflag = false;
  #while(readflag == False):
  #    if (os.path.exists(filename) == False)
  #      sleep(0.005)
  #    else
  xdata, ydata, zdata, cdata  = np.genfromtxt(filename, delimiter = ",", unpack = True)
  # Get Your Data From txt based file

  ax2.clear()
  ax2.set_title(str(i))

  ax2.set_xlim3d([-edge, edge])
  ax2.set_xlabel('X')

  ax2.set_ylim3d([-edge, edge])
  ax2.set_ylabel('Y')

  ax2.set_zlim3d([-edge, edge])
  ax2.set_zlabel('Z')

  ax2.scatter3D(xdata, ydata, zdata, c=cdata);
  #readlag = True
        #print(serialarray)
#End of SerialRead()

#########################################################


#8. Animation Section



print("Plot begins...")
#--
ani = animation.FuncAnimation(fig2, graph_update, frames = np.arange(0,frames+1), blit = False, repeat = False, interval = 500) 

#plt.savefig(figname2)
plt.show()
#---
print("Plot ends...")


#Save the last snapshot
figname2 = os.path.join(parent_path, 'posplot.png')
plt.savefig(figname2)
#plt.savefig(session_name.join('.png'), bbox_inches='tight')
#pylab leaves a generous, often undesirable, whitespace around the image → 'tight'

#Save the animation
vidname = os.path.join(parent_path, 'pos.mp4')
ani.save(vidname)





