import build.pyclientMQ
from build.pyclientMQ import *

print(dir(build.pyclientMQ))


client = clientMQ()

client.init()
client.run()

while(1):
    gws = GymworldState()
    gws.ff.pos.x=2
    
    client.send(gws)