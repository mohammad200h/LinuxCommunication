//  #include "msg.h"
#include "Com/Com.h"
#include <iostream>
using namespace std;


int main(int argc, char *argv[])
{
    Server* server = new Server();
    server->init();
    server->create_run_thread();
    while(1){

        //*******************update gym state for all the envs********************
		/****
		Server serves one client at a time
		we get list of clients that should be served by calling getClientsInQueue
		****/
		vector <boost::uuids::uuid> clients_id = server->getClientsInQueue();
		/*****
		Assign Client to env if client id is not assotiated with any env
		******/
		for(auto  id: clients_id){

			/**
			Pass new state to env so change in Gym state will be 
			reflected in next simulation step
			**/
			// cout<<"clientID:: "<<id<<endl;
			GymworldState gymState = server->getGymStateforId(id);
			server->removeStateFromBuffer(id);
			
		}
        GhostWorldState ghostState;
        vector <pair<boost::uuids::uuid,GhostWorldState>> ghost_vec;
		// cout<<"clients_id::len:: "<<clients_id[0]<<endl;
		// cout<<"clients_id::len:: "<<clients_id.size()<<endl;
		for(int i;i<clients_id.size();i++){
			ghost_vec.push_back(make_pair(clients_id[i],ghostState));
			cout<<"inside loop!"<<endl;
		}
		// cout<<"ghost_vec::len::"<<ghost_vec.size()<<endl;
        //*****************Send Ghost stat to clients*****************************
		//Client that sent new Gym state will be waiting for Ghost state to be send to them
		//After Ghost state is sent to client. it's socket will be removed until ask for new connection from server
		for(auto it = ghost_vec.begin(); it != ghost_vec.end(); ++it){
			auto v_temp = *it;
			boost::uuids::uuid client_id = v_temp.first;
			GhostWorldState ghostState = v_temp.second;
			cout<<"sending.."<<endl;
			server->send( client_id, ghostState);
			//reset ghost state for env
	
		}
		//************************************************************************
        // cout<<"fuck me!"<<endl;

    }

    // server->run();

    return 0;

}
