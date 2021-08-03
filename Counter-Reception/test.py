#from SerialLib import Receiver, ListPorts
from receiver import Receiver
import time
# ListPorts()

print("\n\n")
rnd_port = Receiver("Teensy Rnd Output", "COM8", 19200, Sep = ',')

rnd_port.open()
rnd_port.to_file("./finaltest33.txt")
rnd_port.set_event_counter(10000);


if(rnd_port.is_open()):
    print("YOLO")
    while(rnd_port.EventCounter > 0):
        rnd_port.receive_to_file()

#print(f" Filename: {rnd_port.Filename}")   
rnd_port.status()    
rnd_port.close()