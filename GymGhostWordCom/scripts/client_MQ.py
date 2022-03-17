#This should work after installing the package using setup.py

import pyclientMQ
from pyclientMQ import *

print(dir(pyclientMQ))


client = clientMQ()

client.init()
client.run()

while(1):
    gws = GymworldState()
    gws.ff.pos.x=2
    
    client.send(gws)
