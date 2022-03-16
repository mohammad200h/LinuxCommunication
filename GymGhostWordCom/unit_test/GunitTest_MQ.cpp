//https://www.youtube.com/watch?v=Lp1ifh9TuFI

//cntrl^shift+p run ctest
#include <gtest/gtest.h>  

#include "Com/Com.h"

#include <iostream>
#include <vector>
#include <thread>
#include <cstdlib>
#include <experimental/filesystem>
#include <boost/process.hpp>
namespace bp = boost::process;
using namespace std;

#include <chrono>

using namespace std::chrono;

//#################Searialize and Parser#########

TEST(SearlalizeAndParse,gymState){
    Parser* parser = new Parser();
    Searlalize* searializer = new Searlalize();
    //*******************Generating data on the client side
    GymworldState gym_state ;
    //id
    gym_state.key.id =boost::uuids::random_generator()();
    //ff
    gym_state.ff.pos.x = 1;
    gym_state.ff.pos.y = 2;
    gym_state.ff.pos.z = 3;
    gym_state.ff.orn.r = 4;
    gym_state.ff.orn.p = 5;
    gym_state.ff.orn.y = 6;
    //mf
    gym_state.mf.pos.x = 1;
    gym_state.mf.pos.y = 2;
    gym_state.mf.pos.z = 3;
    gym_state.mf.orn.r = 4;
    gym_state.mf.orn.p = 5;
    gym_state.mf.orn.y = 6;
    //rf
    gym_state.rf.pos.x = 1;
    gym_state.rf.pos.y = 2;
    gym_state.rf.pos.z = 3;
    gym_state.rf.orn.r = 4;
    gym_state.rf.orn.p = 5;
    gym_state.rf.orn.y = 6;
    //th
    gym_state.th.pos.x = 1;
    gym_state.th.pos.y = 2;
    gym_state.th.pos.z = 3;
    gym_state.th.orn.r = 4;
    gym_state.th.orn.p = 5;
    gym_state.th.orn.y = 6;
    //cube
    gym_state.cube.pos.x = 1;
    gym_state.cube.pos.y = 2;
    gym_state.cube.pos.z = 3;
    gym_state.cube.orn.r = 4;
    gym_state.cube.orn.p = 5;
    gym_state.cube.orn.y = 6;
    //**************Searlalizing the data********************
    string json = searializer->gymState(gym_state);
    //**************Parsing searialized data****************

    char data_c[BUFFER_SIZE];
    // cout<<"buffer::"<<buffer<<endl;

    // strcpy(data_c, json.c_str());
    memcpy(&data_c, json.c_str(),BUFFER_SIZE);
    GymworldState parsed_gymState =  parser->parseGymState(data_c);


    EXPECT_TRUE(parsed_gymState.key.id==gym_state.key.id);

    EXPECT_EQ(gym_state.ff.pos.x,parsed_gymState.ff.pos.x);
    EXPECT_EQ(gym_state.ff.pos.y,parsed_gymState.ff.pos.y);
    EXPECT_EQ(gym_state.ff.pos.z,parsed_gymState.ff.pos.z);
    EXPECT_EQ(gym_state.ff.orn.r,parsed_gymState.ff.orn.r);
    EXPECT_EQ(gym_state.ff.orn.p,parsed_gymState.ff.orn.p);
    EXPECT_EQ(gym_state.ff.orn.y,parsed_gymState.ff.orn.y);

    EXPECT_EQ(gym_state.mf.pos.x,parsed_gymState.mf.pos.x);
    EXPECT_EQ(gym_state.mf.pos.y,parsed_gymState.mf.pos.y);
    EXPECT_EQ(gym_state.mf.pos.z,parsed_gymState.mf.pos.z);
    EXPECT_EQ(gym_state.mf.orn.r,parsed_gymState.mf.orn.r);
    EXPECT_EQ(gym_state.mf.orn.p,parsed_gymState.mf.orn.p);
    EXPECT_EQ(gym_state.mf.orn.y,parsed_gymState.mf.orn.y);

    EXPECT_EQ(gym_state.rf.pos.x,parsed_gymState.rf.pos.x);
    EXPECT_EQ(gym_state.rf.pos.y,parsed_gymState.rf.pos.y);
    EXPECT_EQ(gym_state.rf.pos.z,parsed_gymState.rf.pos.z);
    EXPECT_EQ(gym_state.rf.orn.r,parsed_gymState.rf.orn.r);
    EXPECT_EQ(gym_state.rf.orn.p,parsed_gymState.rf.orn.p);
    EXPECT_EQ(gym_state.rf.orn.y,parsed_gymState.rf.orn.y);

    EXPECT_EQ(gym_state.th.pos.x,parsed_gymState.th.pos.x);
    EXPECT_EQ(gym_state.th.pos.y,parsed_gymState.th.pos.y);
    EXPECT_EQ(gym_state.th.pos.z,parsed_gymState.th.pos.z);
    EXPECT_EQ(gym_state.th.orn.r,parsed_gymState.th.orn.r);
    EXPECT_EQ(gym_state.th.orn.p,parsed_gymState.th.orn.p);
    EXPECT_EQ(gym_state.th.orn.y,parsed_gymState.th.orn.y);

    EXPECT_EQ(gym_state.cube.pos.x,parsed_gymState.cube.pos.x);
    EXPECT_EQ(gym_state.cube.pos.y,parsed_gymState.cube.pos.y);
    EXPECT_EQ(gym_state.cube.pos.z,parsed_gymState.cube.pos.z);
    EXPECT_EQ(gym_state.cube.orn.r,parsed_gymState.cube.orn.r);
    EXPECT_EQ(gym_state.cube.orn.p,parsed_gymState.cube.orn.p);
    EXPECT_EQ(gym_state.cube.orn.y,parsed_gymState.cube.orn.y);
}


TEST(SearlalizeAndParse,ghostState){
    Parser* parser = new Parser();
    Searlalize* searializer = new Searlalize();
    //*******************Generating data on the client side
    GhostWorldState ghost_state ;
    //id
    ghost_state.key.id =boost::uuids::random_generator()();
    //ff
    ghost_state.ff =true;
    ghost_state.mf =false;
    ghost_state.rf =false;
    ghost_state.th =false;


    //**************Searlalizing the data********************
    string json = searializer->ghostState(ghost_state);
    //**************Parsing searialized data****************

    char data_c[BUFFER_SIZE];
    // cout<<"buffer::"<<buffer<<endl;

    // strcpy(data_c, json.c_str());
    memcpy(&data_c, json.c_str(),BUFFER_SIZE);
    GhostWorldState parsed_ghostState =  parser->parseGhostState(data_c);

    EXPECT_TRUE(parsed_ghostState.key.id==ghost_state.key.id);

    EXPECT_EQ(ghost_state.ff,parsed_ghostState.ff);
    EXPECT_EQ(ghost_state.mf,parsed_ghostState.mf);
    EXPECT_EQ(ghost_state.rf,parsed_ghostState.rf);
    EXPECT_EQ(ghost_state.th,parsed_ghostState.th);



}

//##################Server#######################
//There seems to be a problem with p_thread and google unit test 
//The code stops working on mutex lock 
TEST(Server_Test,add_client_to_queue_and_remove_client_from_queue){
    Server_MQ* server= new Server_MQ();
    //##########add_client_to_queue############
   
    GymworldState gym_state ;
    //id
    gym_state.key.id =boost::uuids::random_generator()();
    //time step
    milliseconds ms = duration_cast< milliseconds >(
    system_clock::now().time_since_epoch());
    gym_state.key.timestep = ms;

    server->add_client_to_queue(gym_state.key.id);
    /*
    the id should be stored as it is encountered for
    the first time
    */
    EXPECT_TRUE(server->getClientsInQueue().size()==1);
    /*
    the id should not be stored as it is encountered for
    the second time
    */
    server->add_client_to_queue(gym_state.key.id);
    EXPECT_TRUE(server->getClientsInQueue().size()==1);
    //#######remove_client_from_queue########
    server->remove_client_from_queue(gym_state.key.id);
    EXPECT_TRUE(server->getClientsInQueue().size()==0);

}

TEST(Server_Test,add_client_state_to_buffer_and_removeStateFromBuffer){
    Server_MQ* server = new Server_MQ();

    GymworldState gym_state ;
    gym_state.ff.pos.x = 1;
    gym_state.key.id =boost::uuids::random_generator()();
    server->add_client_to_queue(gym_state.key.id);
    //###########add_client_state_to_buffer######

    server->add_client_state_to_buffer(gym_state.key.id,gym_state);
    //checking state is added
    EXPECT_TRUE(server->state_buffer.size()==1);
    EXPECT_EQ(server->state_buffer[0].ff.pos.x,gym_state.ff.pos.x);
    //check state changes but buffer size stays the same
    gym_state.ff.pos.x = 2;
    server->add_client_state_to_buffer(gym_state.key.id,gym_state);
    EXPECT_TRUE(server->state_buffer.size()==1);
    EXPECT_EQ(server->state_buffer[0].ff.pos.x,gym_state.ff.pos.x);
    
    //###########removeStateFromBuffer###########

    server->removeStateFromBuffer(gym_state.key.id);
    EXPECT_TRUE(server->state_buffer.size()==0);
    
}



//##################Client#######################


//######Server and Client Interaction############
//I still dont know who to test system as a whole
//###############################################

 
     
