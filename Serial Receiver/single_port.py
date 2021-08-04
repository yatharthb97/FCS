# coding: UTF-8
# Sample script for reading a single port using Receiver class.
#Author - Yatharth Bhasin (github → yatharthb97)

from receiver import Receiver, ListPorts


ListPorts()
print("\n\n")

rnd_port = Receiver("Teensy Rnd Output", "COM8", 19200, Sep = ',')


rnd_port.to_file("./test18.txt")
rnd_port.set_down_counter(10000);
rnd_port.open()

if(rnd_port.is_open()):
    while(rnd_port.EventCounter > 0):
        rnd_port.receive_to_file()
   

rnd_port.status()
rnd_port.close()
rnd_port.File.close()