//  #include "msg.h"
#include "Com/Com.h"
#include <iostream>
using namespace std;


int main(int argc, char *argv[])
{
    Server_MQ* server = new Server_MQ();
    server->init();
    server->create_run_thread();

    // server->run();
	
	while(1){
        vector <boost::uuids::uuid> clients_id = server->getClientsInQueue();
        boost::uuids::uuid & id = clients_id[0];
        cout<<"\rclients_id::len"<<clients_id.size();
        for(auto & id: clients_id){
	        GymworldState gymState = server->getGymStateforId(id);
            server->removeStateFromBuffer(id);
            GhostWorldState ghostState;
            ghostState.ff = true;
            ghostState.mf = false;
            ghostState.rf = false;
            ghostState.th = false;
            server->send(id,ghostState);
            server->remove_client_from_queue(id);
        }

        




        
    }

    return 0;

}
