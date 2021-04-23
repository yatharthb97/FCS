#do - import exception safety

#Serial Lib Check -------------------------------------------------------
try:
  import serial.tools.list_ports as port_list
except ImportError:
  print ("Import Error - serial.tools.list_ports")
  import subprocess
  import sys

  subprocess.check_call([sys.executable, "-m", "pip", "install", "serial"])
  import serial.tools.list_ports as port_list
#--------------------------------------------------------------------------
#
#
#matplotlib Lib Check -------------------------------------------------------
try:
  import matplotlib.pyplot as plt
except ImportError:
  print ("Import Error - matplotlib")
  import subprocess
  import sys

  subprocess.check_call([sys.executable, "-m", "pip", "install", "matplotlib"])
  import matplotlib.pyplot as plt
#--------------------------------------------------------------------------
#
#
#import animation ---------------------------------------------------------
import matplotlib.animation as animation
#--------------------------------------------------------------------------
#
#
#Global Variables ###################################
D_Sep = ','
COMdefault = "COM8" #do - OS Specific Port default
graphtitle = 'Autocorrelation - Update '
BaudRate = 19200 #Set Accordingly
#####################################################


#List all available ports on machine
ports = list(port_list.comports())
print("Listing all available ports:\n")
for i in ports:
    print (i, "\n")


#take input
#print("Enter the COM Port: ")

COMport = COMdefault
#print("Selected COM port:", COMport)



#Declare the speciic COM Port
s = serial.Serial(port = COMport, timeout = None, baudrate = BaudRate, xonxoff=False, rtscts=False, dsrdtr=True) 

#Setup Log File
LogInit()


#Graph Initialization ####################################

#Produce Figure Object
fig = plt.figure()

#Creating a subplot 
ax1 = fig.add_subplot(1,1,1)

#Labels
plt.xlabel('G(Tau)')
plt.ylabel('Time(MicroSeconds)')
###########################################################



while(1) #---------------------------------------------------------------------

	#Read from the COMPort
	serialstring = s.readline() #Read in a single line
	serialarray = serialstring.split(D_Sep) #Split into the array


	#Update the graph at regular intervals
	ani = animation.FuncAnimation(fig, graph_update, interval=100, blit = true, save_count=200) 
	#Wait for 100 ms before updating & Will only save 200 frames
	
	plt.show()

	Log(serialstring) #Store the received value

	#Save the animation
	ani.save(session_name.join('.mp4'))

#While Loop Ends Here #-------------------------------------------------------


#Save the last snapshot
plt.savefig(session_name.join('png'), bbox_inches='tight')
#pylab leaves a generous, often undesirable, whitespace around the image → 'tight'




#Graph Update Function
def graph_update(i):

    #Clear Graph
    ax1.clear()

    #Draw the new graph
    ax1.plot(xarray, serialarray)

    #Update the title
    plt.title(string(graphtitle.join(i)))
# End of graph_update()

#Setup Logging Resources
def LogInit(): 
	#Empty
# End of LogInit()

 #Logging function
def Log(stringdata):
	#Empty
# End of Log()