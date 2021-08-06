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

from update_enums import UpdateStatus
from counter import UD_Counter


def ListPorts():
    '''List all available ports on the machine.'''
    allports = list(port_list.comports())
    print("Listing all available ports:\n")
    for port in allports:
        print(port)


class Receiver:

	'''Receiver is an object that is associated with a serial port and can receive a stream of data and split it with a seperator (Sep) arguement. The contents of the communication channel will be stored within the receiver object and can be retrived from that object.The object initalizes a blank serial port. The initalization and opening is performed by the Receiver::open() method.'''

	def __init__(self, Unique_Name, Comport, Baudrate = 19200, Sep = ' '):
		'''Constructor that requires a minimum of a Name and COMPORT address.'''
		self.Name = Unique_Name.replace(' ','_')
		self.Com = Comport
		self.Baud = Baudrate
		self.Sep = Sep

		self.InitTime = time.time_ns()
		self.Port = Serial()

		self.EventCounter = UD_Counter()
		self.EventCounter.verbose = False
		self.EventCounter.set_up_counter(0)
		self.EventCounter.verbose = True

		
		self.ReceiveCalls = 0
		self.DecodeErrors = 0
		
		self.MaxGraphSize = 20

		self.Data = []
		self.EventsList = []
		self.DecodeFailureList = []
		
		self.Help = \
		f''' > Resource List: 
			   [ Name, COM, Baud, Sep,
			     Data, EventsList, DecodeFailureList,
			     EventCounter.val(), EventCounter.error(),
			     ReceiveCalls, DecodeErrors, 
			     (File.name, File) (Tempfile.name, Tempfile)]''' #TODO
		
		#Set a temporary file - Append mode, Line buffered
		self.Tempfile = tempfile.TemporaryFile(mode='a', buffering = 1, suffix = ".dat", prefix="SerialData__")


		#Initalize the default file object to the Tempfile object.
		self.File = self.Tempfile 

		#Print Info
		print(f" • Receiver → {self.Name} Created.")
		print(f" • Temporary Filename → {self.File.name}")


	def open(self):
		''' Open communication channel with the set COMPORT and baud rate. Also updates the Init_time. '''
		try:
			self.Port = Serial(port = self.Com, timeout = None, baudrate = self.Baud, xonxoff=False, rtscts=False, dsrdtr=False)
			if self.Port.is_open:
				print(f" • Receiver - {self.Name} | Port - {self.Com}, {self.Port.name} → PORT OPEN.")

		except SerialException:
			print(f" •  ERROR > Receiver - {self.Name} | Port - {self.Com} → Unable to open port.")

		self.InitTime = time.time_ns() #Reset Init_time


	def is_open(self):
		'''Returns True if the specified receiver is open to receive. '''
		return self.Port.is_open


	def close(self):
		''' Closes the communication channel and the corresponding port. However, the resources are not destroyed. It also closes the `File` object. '''
		
		print(f" •  Receiver - {self.Name} | Port - {self.Com} → PORT CLOSED.")
		self.Port.close()

		self.File.flush()
		self.File.close()

	def to_file(self, filename):
		''' Sets the file path for saving data. Upon failure, the file is set to TempFile.'''
		if os.path.exists(filename):
			print(f" • ERROR > The file ** {filename} ** already exists. Ignoring this call.")

		else:
			try:
				#Open in append mode and line buffered mode.
				self.File = open(filename, 'a', buffering = 1)
				print(f" • Writing to file: ** {filename} **")
			
			except FileNotFoundError:
				print(f"• ERROR > Invalid filepath!  Writing to Temp-File → ** {self.File.name} **")


	def status(self):
		'''Prints a preety summary of the port.'''
		print(f''' 
                   • Open -> [{self.Port.is_open}] - {self.Name}
       _,--()      • Filename:     {self.File.name}
  ( )-'-.------|>  • Events:       {self.EventsList}
   "   `--[]       • EventCounter: {self.EventCounter.val()}           
                   • Total Read Attempts: {self.ReceiveCalls} (Total Polling Events)
      P O R T      • Actual Read Events:  {sum(self.EventsList)}
    S T A T U S    • Exceptions: {self.DecodeErrors} - {self.DecodeFailureList}''')
		
#Graphing section [INCOMPLETE]

 
	
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



					
					
