# coding: UTF-8
# Class defination of `class Receiver`.
# Author - Yatharth Bhasin (github → yatharthb97)
# Assumes a 3-wire null modem serial communication standard.

import serial.tools.list_ports as port_list
from serial import Serial
from serial import SerialException

import matplotlib.pyplot as plt

import time
import os
import tempfile



#TODO - Think of a fast numpy based solution. The class needs to be interfaced to allow for objects like blockReceiver, variable-length-receiver, fixedlength-rceiver, etc

def ListPorts():
    '''List all available ports on the machine.'''
    ports = list(port_list.comports())
    print("Listing all available ports:\n")
    for port in ports:
        print(port)
	

class Receiver:

	'''Receiver is an object that is associated with a serial port and can receive a stream of data and split it with a seperator (Sep) arguement. The contents of the communication channel will be stored within the receiver object and can be retrived from that object.The object initalizes a blank serial port. The initalization and opening is performed by the Receiver::open() method.'''

	def __init__(self, Rec_Name, COMPORT, Baudrate = 19200, Sep = ' '):
		'''Constructor that requires a minimum of a Name and COMPORT address.'''
		self.Name = Rec_Name
		self.COM = COMPORT
		self.Baud = Baudrate
		self.Sep = Sep

		self.Init_time = time.time_ns()
		self.port = Serial()

		self.EventCounter = 0
		self.Receive_Calls = 0
		self.DecodeErrors = 0
		self.MaxGraphSize = 20

		self.Data = []
		self.EventsList = []
		self.DecodeFailureList = []
		
		self.Help = " > Resource List: [Name, COM, Baud, Data, EventCounter, (File.name, File) Tempfile Object]" #TODO
		
		self.Tempfile = tempfile.TemporaryFile(mode='w', buffering=-1, suffix = ".dat", prefix="SerialData__") #Set a temporary file

		self.File = self.Tempfile #Initalize the default file object to the Tempfile object.

		#Print Info
		print(f" • Receiver → {self.Name} Created.")
		print(f" • Temporary Filename → {self.File.name}")

	def set_down_counter(self, iterations: int):
		''' Set the number of allowed iterations'''
		
		assert iterations > 0 , 'Accepts only unsigned integers.'
		
		self.EventCounter = iterations
		print(f" • Allowed Events set: {self.EventCounter}")
		self.EventsList.append(self.EventCounter)

	def open(self):
		''' Open communication channel with the set COMPORT and baud rate. Also updates the Init_time. '''
		try:
			self.port = Serial(port = self.COM, timeout = None, baudrate = self.Baud, xonxoff=False, rtscts=False, dsrdtr=False)
			if self.port.is_open:
				print(f" • Receiver - {self.Name} | Port - {self.COM}, {self.port.name} → PORT OPEN.")

		except SerialException:
			print(f" •  ERROR > Receiver - {self.Name} | Port - {self.COM} → Unable to open port.")

		self.Init_time = time.time_ns() #Reset Init_time


	def is_open(self):
		'''Returns True if the specified receiver is open. '''
		return self.port.is_open


	def close(self):
		''' Closes the communication channel and the corresponding port. However, the resources are not destroyed.'''
		self.port.close()
		print(f" •  Receiver - {self.Name} | Port - {self.COM} → PORT CLOSED.")
		self.File.close()

	def to_file(self, filename):
		''' Sets the file path for saving data. Upon failure, the file is set to TempFile.'''
		if os.path.exists(filename):
			print(f" • ERROR > The file ** {filename} ** already exists. Ignoring this call.")

		else:
			try:
				self.File = open(filename, 'w')
				print(f" • Writing to file: ** {filename} **")
			
			except FileNotFoundError:
				print(f"• ERROR > Invalid filepath!  Writing to Temp-File → ** {self.File.name} **")


	#  -------------------------- Update/Receive functions -------------------------- #
	''' All receive functions will decrease the event counter by 1. If file is provided,
	    the raw string is directly dumped in the file irrespective of Decoding Exceptio
	    -ns.'''

	def receive_to_file(self):
		''' Reads a vector or scalar to file.  Optimized. Does not update the Data attribute.'''
		self.Receive_Calls = self.Receive_Calls + 1
		if(self.port.in_waiting > 0):
			    
			    serialdata = self.port.readline() #Read in a single line
			    self.EventCounter = self.EventCounter - 1
			    try:
			    	s_string = serialdata.decode('utf-8') #Decode to ASCII
			    	s_string_clean = s_string.rstrip('\r\n') #Remove carriage-return character
			    	#s_string_clean = s_string_clean.rstrip('\r') #Remove endline character
			    	s_string_clean +='\n'
			    	self.File.write(s_string_clean)
			    	return True

			    except:
			    	self.DecodeErrors = self.DecodeErrors + 1;
			    	self.DecodeFailureList.append(serialdata)
		return False
			    

	def receive_vector(self):
	    ''' Wait and receive data from the COM port. '''
	    self.Receive_Calls = self.Receive_Calls + 1
	    if(self.port.in_waiting > 0):
	    	try: 
	    		self.EventCounter = self.EventCounter - 1

	    		serialdata = self.port.readline() #Read in a single line
	         
	    		s_string = serialdata.decode('ascii') #Decode to ASCII
	    		s_string_clean = s_string.rstrip('\r\n') #Remove endline character
	    		serialarray_str = s_string_clean.split(self.Sep) #Split into the array

	    		self.Data = [float(numeric_string) for numeric_string in serialarray_str]
	    		
	    		s_string_clean +='\n'
	    		self.File.write(s_string_clean)
	    		return True

	    	except:
	    		self.DecodeErrors = self.DecodeErrors + 1
	    		self.DecodeFailureList.append(serialdata)

	    return False



	def receive_and_print(self):
		''' Reads one vector and prints with the time reception time in milliseconds. '''
		self.Receive_Calls = self.Receive_Calls + 1
		if(self.port.in_waiting > 0):
			try: 
				self.EventCounter = self.EventCounter - 1

				serialdata = self.port.readline() #Read in a single line
				receive_time = float(time.time_ns() - self.Init_time) // 1000 
				#flooring last digit (1ms digit)       

				s_string = serialdata.decode('ascii') #Decode to ASCII
				s_string_clean = s_string.rstrip('\r\n') #Remove endline character
				serialarray_str = s_string_clean.split(self.Sep) #Split into the array
				
				s_string_clean +='\n'
				self.File.write(s_string_clean)
				
				self.Data = [float(numeric_string) for numeric_string in serialarray_str]
				print(f" {chalk.blue(self.EventCounter)}.  {receive_time} ms → {self.Data}")
				return True

			except:
				self.DecodeErrors = self.DecodeErrors + 1
				self.DecodeFailureList.append(serialdata)

		return False


	def receive_and_append(self):
		''' Receives one point of data and then appends it to the Data List.'''
		
		self.Receive_Calls = self.Receive_Calls + 1
		if(self.port.in_waiting > 0):

			try:
			    serialdata = self.port.readline()
			    self.EventCounter = self.EventCounter - 1
			    s_string = serialdata.decode('ascii')
			    s_string_clean = s_string.rstrip('\r\n') #Remove endline character
			    
			    #data_points = float(s_string_clean)
			    self.Data.append([float(numeric_string) for numeric_string in s_string_clean])
			    #self.Data.append(data_point)
			    self.Data = self.Data[-self.MaxGraphSize:] #Resize

			    s_string_clean +='\n'
			    self.File.write(s_string_clean)
			    return True

			except:	
				self.DecodeErrors = self.DecodeErrors + 1
				self.DecodeFailureList.append(serialdata)
			return False

	def status(self):
		'''Prints a preety summary of the port.'''
		print(f''' 
                   • Open -> [{self.port.is_open}] - {self.Name}
       _,--()      • Filename:   {self.File.name}
  ( )-'-.------|>  • Events:     {self.EventsList}
   "     `--[]     • Exceptions: {self.DecodeErrors}
                   •             {self.DecodeFailureList}
      P O R T      • Total Read Attempts: {self.Receive_Calls} (Total Polling Events)
    S T A T U S    • Actual Read Events:  {sum(self.EventsList)}''')

	#Graphing section [INCOMPLETE]

    	# Utility functions [PRIVATE SCOPE]
	
	def set_graph_canvas(self):
		'''Sets the graph axes, title and other settings.'''
		self.ax.set_xlabel('Index → ')
		self.ax.set_ylabel('Value → ')
		self.ax.set_title(f"`{self.Name}` Receiver - Data Graph")


	def graph_update(self, receive_fn, delay_ms = 0,  no_of_calls = 1):
		''' Calls the function and updates the graph object. '''

		iterations_left = no_of_calls
		
		while(iterations_left > 0):
			updates = receive_fn() #Call function
			
			#---------
			if(updates == True):
				self.ax.clear()
				set_graph_canvas()
				self.ax.plot(np.arange(0, len(self.Data)), self.Data)
				plt.draw()
				#---------

			iterations_left = iterations_left - 1


	def open_graph(self):
		''' [INCOMPLETE] This method opens a gui window that updates in real time as new data is received. '''
		self.fig = plt.figure()
		self.ax = self.fig.add_subplot(111)
		self.set_graph_canvas()
		plt.ion() #Enable automatic redrawing
		plt.show(block=False) #open graph



					
					
