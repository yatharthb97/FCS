# coding: UTF-8
# Sample script for reading multiple ports without use of parallelism using Receiver class.
#Author - Yatharth Bhasin (github → yatharthb97)


from receiver import Receiver, ListPorts

ListPorts()
print("\n\n")

session = "Test1"
print(f"Session → Test1")

print("\n\n")

#Initalise ports
ports = [Receiver("Port 1", "COM8", Baudrate = 115200, Sep = ','),
		 Receiver("Port 2", "COM9", Baudrate = 115200, Sep = ','),
		 Receiver("Port 3", "COM10", Baudrate = 115200, Sep = ','),
		 Receiver("Port 4", "COM11", Baudrate = 115200, Sep = ',')]


#Set file and max counter values
for port in ports:
	filename = "./" + session + '_' + port.Name + ".txt"
	port.to_file(filename)
	port.set_down_counter(10000);

#Open Ports
for port in ports:
	port.open()

#Check if the ports are open
open_status = sum([port.is_open() for port in ports])

#If ports are open → Read sequentially from each port
if(open_status == len(ports)):
    for port in ports:
        if port.EventCounter > 0:
        	port.receive_to_file()
else:
	print(" • ERROR > Some ports are not open!")

#Close ports
for port in ports:
	port.close()

#Print status of ports
for port in ports:
	port.status()