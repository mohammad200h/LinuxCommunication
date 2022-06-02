#This should work after installing the package using setup.py

import pyserverMQ
from pyserverMQ import *

# print(dir(pyserverMQ))


ghostState = GhostWorldState()
ghostState.ff = True
ghostState.mf = False
ghostState.rf = False
ghostState.th = False




server = serverMQ()

server.init()
server.run()

while(1):

    
   clients_id = server.getClientsInQueue()
   # print("clients_id:: ",clients_id)

   for id in clients_id:
      # print("id:: ",id)
      gymState = server.getGymStateforId(id)
     
      
      # sucess = server.send_using_key(key,ghostState)
      sucess= server.send_using_id(id,ghostState)
      server.removeStateFromBuffer(id)
      server.remove_client_from_queue(id)

# https://stackoverflow.com/questions/67837504/how-to-debug-python-core-dump
# https://www.youtube.com/watch?v=bXEXE6ywzSA