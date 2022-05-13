//  #include "msg.h"
#include "Com/Com.h"
#include <iostream>
using namespace std;



/*##################

This program simulate serving different clients
###################*/

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
            cout<<"server::id::"<<uuid_s(id)<<endl;
	        GymworldState gymState = server->getGymStateforId(id);
            cout<<"server::gymState::ff::pos::z::"<<gymState.ff.pos.z<<endl;
           
            GhostWorldState ghostState;
            ghostState.ff = true;
            ghostState.mf = false;
            ghostState.rf = false;
            ghostState.th = false;
            service_keys key = server->getKeyId(id);
            server->send(key,ghostState);
            cout<<"I am inside the server_MQ.cpp"<<endl;
            server->clean_after_serving_client(id);
            
        }

        




        
    }

    return 0;

}
