
import serial.tools.list_ports as port_list
from serial import Serial
from serial import SerialException
import matplotlib.pyplot as plt
from yachalk import chalk
import numpy as np
import time
import os
import tempfile



#TODO - Think of a fast numpy based solution. The class needs to be interfaced to allow for objects like blockReceiver, variable-length-receiver, fixedlength-rceiver, etc

#Receive in binary mode and decode to ASCII


class Receiver:

	'''Receiver is an object that is associated with a port and a baud rae and can receive a stream of data and split it with a seperator arguement. The countents of the communication channel will be stored within the Receiver object and can be retrived from that object.The object initalizes a blank serial port. '''

	def __init__(self, Obj_Str_Name, COMPORT, Baudrate, Sep = ' '):
		self.Name = Obj_Str_Name
		self.COM = COMPORT
		self.Baud = Baudrate
		self.Data = []
		self.Sep = Sep
		self.Init_time = time.time_ns()
		self.port = Serial()
		self.file_flag = False
		self.EventCounter = 0
		self.Receive_Calls = 0
		self.DecodeErrors = 0
		self.MaxGraphSize = 20
		self.EventsList = []
		self.DecodeFailureList = []
		self.Help = " > Resource List: [Name, COM, Baud, Data, EventCounter, (File.name, File) Tempfile Object]"
		
		self.Tempfile = tempfile.TemporaryFile(mode='w', buffering=-1, suffix = ".dat", prefix="SerialData__")
		self.File = self.Tempfile

		print(f" • Receiver → {self.Name} Created.")
		print(f" • Temporary Filename → {self.File.name}")

	def set_down_counter(self, iterations):
		''' Set the number of allowed iterations'''
		#assert(iterations <= 1)
		self.EventCounter = iterations
		print(f" • Allowed Events set: {self.EventCounter}")
		self.EventsList.append(self.EventCounter)

	def open(self):
		''' Open communication channel with the set PORT and baud rate. '''
		try:
			self.port = Serial(port = self.COM, timeout = None, baudrate = self.Baud, xonxoff=False, rtscts=False, dsrdtr=False)
			if self.port.is_open:
				print(f" • Receiver - {self.Name} | Port — {self.COM} → PORT OPEN.")
				print(f" • Reading → {chalk.blue(self.port.name)} .") #Prints the default name

		except SerialException:
			print(f" •  ERROR > Receiver - {self.Name} | Port — {self.COM} → Unable to open port.")

	def is_open(self):
		'''Returns True if the specified receiver is open. '''
		return self.port.is_open

	def close(self):
		''' Closes the communication channel and the corresponding port. '''
		self.port.close()
		print(f" •  Receiver - {self.Name} | Port — {self.COM} → PORT CLOSED.")


	def to_file(self, filename):
		if os.path.exists(filename):
			print(f" • The file {chalk.red(filename)} already exists. Ignoring this call.")

		else:
			try:
				self.file_flag = True
				self.File = open(filename, 'w')
				print(f" • Writing to file:  {chalk.blue(filename)}")
			except FileNotFoundError:
				print("Invalid filepath!")
				print(f" • Writing to Temp-File → {self.File.name}")
	def open_graph(self):
		''' This method opens a gui window that updates in real time as new data is received. '''
		self.fig = plt.figure()
		self.ax = self.fig.add_subplot(111)
		self.set_graph_canvas()
		plt.ion() #Enable automatic redrawing
		plt.show(block=False) #open graph


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
			    	s_string = serialdata.decode('ascii') #Decode to ASCII
			    	s_string_clean = s_string.rstrip('\r\n') #Remove carriage-return character
			    	#s_string_clean = s_string_clean.rstrip('\n') #Remove endline character
			    	s_string_clean +='\n'
			    	self.File.write(s_string_clean)
			    	return True

			    except UnicodeDecodeError:
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

	    	except UnicodeDecodeError:
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

			except UnicodeDecodeError:
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
			    
			    data_point = float(s_string_clean)
			    self.Data.append(data_point)
			    self.Data = self.Data[-self.MaxGraphSize:]

			    s_string_clean +='\n'
			    self.File.write(s_string_clean)
			    return True

			except UnicodeDecodeError:	
				self.DecodeErrors = self.DecodeErrors + 1
				self.DecodeFailureList.append(serialdata)
			return False

	def graph_update(self, receive_fn, delay_us = 0,  no_of_calls = 1):
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
			#time.sleep(delay_us * 1e-6) #Sleep for given microseconds




	# Utility functions [PRIVATE SCOPE]
	
	def set_graph_canvas(self):
		'''Sets the graph axes, title and other settings.'''
		self.ax.set_xlabel('Index → ')
		self.ax.set_ylabel('Value → ')
		self.ax.set_title(f"`{self.Name}` Receiver - Data Graph")

	def status(self):
		print(f''' 
                   • Open -> [{self.port.is_open}] - {self.Name}
       _,--()      • Filename:   {self.File.name}
  ( )-'-.------|>  • Events:     {self.EventsList}
   "     `--[]     • Exceptions: {self.DecodeErrors}
                   •             {self.DecodeFailureList}
      P O R T      • Total Read Attempts: {self.Receive_Calls} (Total Polling Events)
    S T A T U S    • Actual Read Events:  {sum(self.EventsList)}''')
					
					
