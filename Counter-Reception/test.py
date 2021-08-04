#from SerialLib import Receiver, ListPorts
from receiver import Receiver
import time
# ListPorts()

print("\n\n")
rnd_port = Receiver("Teensy Rnd Output", "COM8", 115200, Sep = ',')

rnd_port.open()
rnd_port.to_file("./finaltest51.txt")
rnd_port.set_down_counter(10000);


if(rnd_port.is_open()):
    print("YOLOX")
    while(rnd_port.EventCounter > 0):
        rnd_port.receive_and_print()

#print(f" Filename: {rnd_port.Filename}")   
rnd_port.status()    
rnd_port.close()