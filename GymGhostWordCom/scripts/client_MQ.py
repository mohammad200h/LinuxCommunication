#This should work after installing the package using setup.py

import pyclientMQ
from pyclientMQ import *

# print(dir(pyclientMQ))


gym_ws = GymworldState()
 


gym_ws.ff.pos.x=2

client = clientMQ()

client.init()
client.run()

while(1):

    
   sucess =  client.send(gym_ws)
   print("send::sucess:: ",sucess)
   ghost_ws = client.getGhostState()
   print("ghost_ws::type:: ",type(ghost_ws))
   print("ghost_ws::ff:: ",ghost_ws.ff)
   print("ghost_ws::mf:: ",ghost_ws.mf)
   print("ghost_ws::rf:: ",ghost_ws.rf)
   print("ghost_ws::th:: ",ghost_ws.th)
