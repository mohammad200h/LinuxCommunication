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
   print("clients_id:: ",clients_id)

   for id in clients_id:
      # print("id:: ",id)
      gymState = server.getGymStateforId(id)
      key = server.getKeyId(id)
      print("key:: ",key)
      # sucess = server.send_using_key(key,ghostState)
      sucess= server.send_using_id(id,ghostState)
      server.clean_after_serving_client(id)


