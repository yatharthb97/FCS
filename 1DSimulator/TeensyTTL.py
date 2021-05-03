#!/usr/bin/env python3
# coding: utf-8



#1. Import Block ########################################################

#Serial Lib -------------------------------------------------------
import serial.tools.list_ports as port_list
from serial import Serial
#--------------------------------------------------------------------------
#
#
#matplotlib Lib-------------------------------------------------------
import matplotlib.pyplot as plt
from matplotlib import animation, rc
#--------------------------------------------------------------------------

print("Import Complete")


##############################################################################




#2. Global Variables ###################################
D_Sep = ','
COMdefault = "COM8" #do - OS Specific Port default
graphtitle = 'TTL Output'
BaudRate = 19200 #Set Accordingly
session_name = "Test"

#Data Arrays
xs = []
ys = []
########################################################




#4. Comm Port Setting #################################

#List all available ports on machine
ports = list(port_list.comports())
print("Listing all available ports:\n")
for i in ports:
    print (i, "\n")


#take input
#print("Enter the COM Port: ")

COMport = COMdefault
print("COMPort selected: ", COMport)
#print("Selected COM port:", COMport)



#Declare the speciic COM Port
s_port = Serial(port = COMport, timeout = None, baudrate = BaudRate, xonxoff=False, rtscts=False, dsrdtr=False) 

########################################################################################

#5. SerialRead Function ########################

def SerialRead():       
    #Read from the s_port
    if(s_port.in_waiting > 0):
        #read(size) – This will read n number of bytes from the serial port
        
        temp = s_port.read(1) #Read 1byte of data
        temp2 = temp.decode('ascii') #Decode to ASCII
        
        ys.append(temp2)
        xs = xs[-20:]
        ys = ys[-20:]
        
#End of SerialRead()

#########################################################

#Produce Figure Object
fig = plt.figure()

#Creating a subplot 
ax = fig.add_subplot(1,1,1)
ax.set_ylim((0, 1))

#Labels
plt.xlabel('TTL')
plt.ylabel('Time (MicroSeconds)')
###########################################################

#7. Graph Update Function ###############################
def graph_update(i):

    SerialRead()
    
    #Clear Graph
    ax.clear()

    #Draw the new graph
    global xs, ys
    ax.plot(xs, ys)

    #Update the title
    #plt.title(graphtitle.join(str(i)))
    #plt.show()
    
# End of graph_update()
#########################################################


#8. Animation Section

#Open Port
if(s_port.isOpen() == False):
    s_port.open()

print("Read begins...")
#--
ani = animation.FuncAnimation(fig, graph_update, frames = 200, interval=100, blit = False, save_count=200) 
        #Wait for 100 ms before updating & Will only save 200 frames
plt.show()
#---
print("Read ends...")


#Save the last snapshot
plt.savefig(session_name.join('.png'), bbox_inches='tight')
#pylab leaves a generous, often undesirable, whitespace around the image → 'tight'

#Save the animation
#ani.save(session_name.join('.mp4'),writer=writer)


#Close port
s_port.close()
