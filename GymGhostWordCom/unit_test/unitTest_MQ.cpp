#include <iostream>
#include "Com/colormod.h"

#include "Com/Com.h"

#include <iostream>
#include <vector>
#include <thread>
#include <cstdlib>
#include <experimental/filesystem>
#include <boost/process.hpp>


using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
using std::chrono::system_clock;


namespace bp = boost::process;
using namespace std;

#include <chrono>

using namespace std::chrono;



Color::Modifier red(Color::FG_RED);
Color::Modifier green(Color::FG_GREEN);
Color::Modifier def(Color::FG_DEFAULT);

typedef bool (*BoolFunc) ();

#define EXPECT_TRUE(x) { if (!(x)) std::cout << red << __FUNCTION__ << " failed on line " << __LINE__ << def <<std::endl; }

#define EXPECT_EQ(x,y) { if (!(x==y)) std::cout << red << __FUNCTION__ << " failed on line " << __LINE__ << def <<std::endl; }

#define EXPECT_num_EQ(x,y) { if (!((int)x-y<0.0001)) std::cout << red << __FUNCTION__ << " failed on line " << __LINE__ << def <<std::endl; }

#define EXPECT_EQ_STR(x,y) { if (x.compare(y)) std::cout << red << __FUNCTION__ << " failed on line " << __LINE__ << def <<std::endl; }

//######Uitlity Functions############

// double check_if_bool_vecs_are_equal()
 vector <int> decToBinary(int n)
{
    // array to store binary number
    vector <int> binaryNum{0,0,0,0};
 
    // counter for binary array
    int i = 0;
    while (n > 0) {
 
        // storing remainder in binary array
        binaryNum[i] = n % 2;
        n = n / 2;
        i++;
    }
 
    return binaryNum;
}

string vector_str_to_bool(vector <bool> vec){
    std::stringstream ss;
    for(size_t i = 0; i < vec.size(); ++i)
    {
      if(i != 0)
        ss << ",";
      ss << int(vec[i]);
    }
    std::string s = ss.str();
    return s;
}

double rand_double(){
    return ((double)rand())/((double)RAND_MAX);
}

double generate_random_num(){
    double sum = 0;
    for(int i=0;i<7;i++){
        sum +=rand_double();
    }
    return sum;
}

double generate_unique_random_number(vector<double>& history){
    //I am paranoid so i use a history to make sure the number was not
    // generated before

    //generate 7 decimal point and sum them together
    double num = generate_random_num();

    //check the number is not previously generated
    while(std::find(history.begin(), history.end(), num) != history.end()){
         num = generate_random_num();
    }
    history.push_back(num);
    return num;

}

vector <bool> generate_random_bool_vec(int n){

    /*
    We can generate 16 combination for 0,0,0,0 2^4
    */
    
    vector <int> binary;
    vector <bool> candiate;

    //generating
  
    binary =  decToBinary(n);
   

    vector <bool> binary_bool;
    for( int b:binary){
        binary_bool.push_back(bool(b));
    }
    

    return binary_bool;
    
}

GymworldState get_random_gym_state(){
    GymworldState gym_state;
    
    vector<double> history;
    //client send Gym state to server

    gym_state.ff.pos.x = generate_unique_random_number(history);
    gym_state.ff.pos.y = generate_unique_random_number(history);
    gym_state.ff.pos.z = generate_unique_random_number(history);
    gym_state.ff.orn.r = generate_unique_random_number(history);
    gym_state.ff.orn.p = generate_unique_random_number(history);
    gym_state.ff.orn.y = generate_unique_random_number(history);

    gym_state.mf.pos.x = generate_unique_random_number(history);
    gym_state.mf.pos.y = generate_unique_random_number(history);
    gym_state.mf.pos.z = generate_unique_random_number(history);
    gym_state.mf.orn.r = generate_unique_random_number(history);
    gym_state.mf.orn.p = generate_unique_random_number(history);
    gym_state.mf.orn.y = generate_unique_random_number(history);

    gym_state.rf.pos.x = generate_unique_random_number(history);
    gym_state.rf.pos.y = generate_unique_random_number(history);
    gym_state.rf.pos.z = generate_unique_random_number(history);
    gym_state.rf.orn.r = generate_unique_random_number(history);
    gym_state.rf.orn.p = generate_unique_random_number(history);
    gym_state.rf.orn.y = generate_unique_random_number(history);

    gym_state.th.pos.x = generate_unique_random_number(history);
    gym_state.th.pos.y = generate_unique_random_number(history);
    gym_state.th.pos.z = generate_unique_random_number(history);
    gym_state.th.orn.r = generate_unique_random_number(history);
    gym_state.th.orn.p = generate_unique_random_number(history);
    gym_state.th.orn.y = generate_unique_random_number(history);

    return gym_state;
}

GhostWorldState get_random_ghost_state(int n){
    GhostWorldState ghost_state;

    
    vector <bool> random = generate_random_bool_vec(n);

    ghost_state.ff = random[0];
    ghost_state.mf = random[1];
    ghost_state.rf = random[2];
    ghost_state.th = random[3];

    return ghost_state;
}

void run_gym_state_test(int line,GymworldState gym_state_c,GymworldState gym_state_s){
    cout<< "runing test at line::"<<line<<endl;
            
    EXPECT_num_EQ(gym_state_c.ff.pos.x , gym_state_s.ff.pos.x) 
    EXPECT_num_EQ(gym_state_c.ff.pos.y , gym_state_s.ff.pos.y)
    EXPECT_num_EQ(gym_state_c.ff.pos.z , gym_state_s.ff.pos.z)
    EXPECT_num_EQ(gym_state_c.ff.orn.r , gym_state_s.ff.orn.r)
    EXPECT_num_EQ(gym_state_c.ff.orn.p , gym_state_s.ff.orn.p)
    EXPECT_num_EQ(gym_state_c.ff.orn.y , gym_state_s.ff.orn.y)

    EXPECT_num_EQ(gym_state_c.mf.pos.x , gym_state_s.mf.pos.x) 
    EXPECT_num_EQ(gym_state_c.mf.pos.y , gym_state_s.mf.pos.y)
    EXPECT_num_EQ(gym_state_c.mf.pos.z , gym_state_s.mf.pos.z)
    EXPECT_num_EQ(gym_state_c.mf.orn.r , gym_state_s.mf.orn.r)
    EXPECT_num_EQ(gym_state_c.mf.orn.p , gym_state_s.mf.orn.p)
    EXPECT_num_EQ(gym_state_c.mf.orn.y , gym_state_s.mf.orn.y)
            
    EXPECT_num_EQ(gym_state_c.rf.pos.x , gym_state_s.rf.pos.x) 
    EXPECT_num_EQ(gym_state_c.rf.pos.y , gym_state_s.rf.pos.y)
    EXPECT_num_EQ(gym_state_c.rf.pos.z , gym_state_s.rf.pos.z)
    EXPECT_num_EQ(gym_state_c.rf.orn.r , gym_state_s.rf.orn.r)
    EXPECT_num_EQ(gym_state_c.rf.orn.p , gym_state_s.rf.orn.p)
    EXPECT_num_EQ(gym_state_c.rf.orn.y , gym_state_s.rf.orn.y)

    EXPECT_num_EQ(gym_state_c.th.pos.x , gym_state_s.th.pos.x) 
    EXPECT_num_EQ(gym_state_c.th.pos.y , gym_state_s.th.pos.y)
    EXPECT_num_EQ(gym_state_c.th.pos.z , gym_state_s.th.pos.z)
    EXPECT_num_EQ(gym_state_c.th.orn.r , gym_state_s.th.orn.r)
    EXPECT_num_EQ(gym_state_c.th.orn.p , gym_state_s.th.orn.p)
    EXPECT_num_EQ(gym_state_c.th.orn.y , gym_state_s.th.orn.y)
}

void run_ghost_state_test(int line,GhostWorldState ghost_state_c,GhostWorldState ghost_state_s){
    cout<< "runing test at line::"<<line<<endl;

    EXPECT_EQ(ghost_state_c.ff,ghost_state_s.ff)
    EXPECT_EQ(ghost_state_c.mf,ghost_state_s.mf)
    EXPECT_EQ(ghost_state_c.rf,ghost_state_s.rf)
    EXPECT_EQ(ghost_state_c.th,ghost_state_s.th)
}

//#################Searialize and Parser#########
bool Test1(){
    //***************************
    //SearlalizeAndParse,gymState
    //***************************

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

    std::cout << green << "Test 1 Passed"  << def <<std::endl;
    return true;


}

bool Test2(){
    //***************************
    //SearlalizeAndParse,ghostState
    //***************************
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

    std::cout << green << "Test 2 Passed"  << def <<std::endl;
    return true;

}

//##################Server #######################

bool Test3(){
    //***************************
    //Server_Test,add_client_to_queue_and_remove_client_from_queue
    //***************************

    Server_MQ* server= new Server_MQ();
    //##########add_client_to_queue############
   
    GymworldState gym_state ;
    //id
    gym_state.key.id =boost::uuids::random_generator()();
    //time step
    string ms =  get_current_time_str();
    gym_state.key.request_timestep = ms;

    server->add_client_to_queue(gym_state.key.id);
    /*
    the id should be stored as it is encountered for
    the first time
    */
    // EXPECT_TRUE( server->getClientsInQueue().size()==1);
    /*
    the id should not be stored as it is encountered for
    the second time
    */
   
    server->add_client_to_queue(gym_state.key.id);
    int num_client_in_queue = server->getClientsInQueue().size();
    cout<<"Test3::num_client_in_queue"<<num_client_in_queue<<endl;
    EXPECT_TRUE(num_client_in_queue==1);
    //#######remove_client_from_queue########
    server->remove_client_from_queue(gym_state.key.id);
    EXPECT_TRUE( server->getClientsInQueue().size()==0);
    
    std::cout << green << "Test 3 Passed"  << def <<std::endl;

    delete server;
    
    return true;

}
bool Test4(){
    //***************************
    //Server_Test,add_client_state_to_buffer_and_removeStateFromBuffer
    //***************************
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
    // EXPECT_TRUE(server->state_buffer.size()==1);
    // EXPECT_EQ(server->state_buffer[0].ff.pos.x,gym_state.ff.pos.x);
    
    //###########removeStateFromBuffer###########

    server->removeStateFromBuffer(gym_state.key.id);
    // EXPECT_TRUE(server->state_buffer.size()==0);

    std::cout << green << "Test 4 Passed"  << def <<std::endl;

    delete server;
    return true;
}

bool Test5(){

    /*
    Testing the id of client is same as the client being served
    */

    Server_MQ* server = new Server_MQ();
    Client_MQ* client = new Client_MQ();
    

    server->init();
    server->create_run_thread();

    client->init();
    client->create_run_thread();
    

    boost::uuids::uuid client_id;
    boost::uuids::uuid client_being_served_id;

    bool succ_s =false;
    GymworldState gym_state_s;
    GhostWorldState ghost_state_s;

    bool client_successfully_send_state = false;
    GymworldState gym_state_c;
    GhostWorldState ghost_state_c;

    //client send Gym state to server
    gym_state_c.ff.pos.z = 5;
    client_successfully_send_state = client->send(gym_state_c);

    
   

    while(client_successfully_send_state){
        vector <boost::uuids::uuid> clients_id = server->getClientsInQueue();
        if (clients_id.size()==1){
            // cout<<"client_id::"     <<uuid_s(client->get_id())<<endl;
            // cout<<"clients_id[0]::" <<uuid_s(clients_id[0])<<endl;
            EXPECT_EQ(client->get_id() ,clients_id[0]);
            break;
        }

    }
    
    std::cout << green << "Test 5 Passed"  << def <<std::endl;
    delete client;
    delete server;
    return true;
}

bool Test6(){

    /*
    Testing the server is reciving correct data that is coming from client
    */

    Server_MQ* server = new Server_MQ();
    Client_MQ* client = new Client_MQ();
    

    server->init();
    server->create_run_thread();

    client->init();
    client->create_run_thread();
    

    boost::uuids::uuid client_id;
    boost::uuids::uuid client_being_served_id;

    bool succ_s =false;
    GymworldState gym_state_s;
    GhostWorldState ghost_state_s;

    bool client_successfully_send_state = false;
    GymworldState gym_state_c;
    GhostWorldState ghost_state_c;


    vector<double> history;
    //client send Gym state to server

    gym_state_c = get_random_gym_state();

    client_successfully_send_state = client->send(gym_state_c);


    while(client_successfully_send_state){
        vector <boost::uuids::uuid> clients_id = server->getClientsInQueue();
        if (clients_id.size()==1){
            boost::uuids::uuid & id = clients_id[0];
            gym_state_s = server->getGymStateforId(id);
            server->removeStateFromBuffer(id);
           
            server->remove_client_from_queue(id);

            run_gym_state_test(444,gym_state_c,gym_state_s);
            break;
        }

    }
    
    std::cout << green << "Test 6 Passed"  << def <<std::endl;

    delete client;
    delete server;
    return true;
}

bool Test7(){
    /*
    Making sure the server can correctly recive data from multiple clients
    */
    vector <bool> clients_succ;
   
    Server_MQ* server = new Server_MQ();
    
    Client_MQ* client_1 = new Client_MQ();
    Client_MQ* client_2 = new Client_MQ();
    Client_MQ* client_3 = new Client_MQ();
    Client_MQ* client_4 = new Client_MQ();
    vector <Client_MQ*> client_list{client_1,client_2,client_3,client_4};
    
    server->init();
    server->create_run_thread();

    for(Client_MQ* c:client_list){
        c->init();
        c->create_run_thread();
    }
    // initalizing 
    GymworldState gym_state_s;
    GhostWorldState ghost_state_s;

    vector <GymworldState> gym_state_c_vect;
    for(int i=0;i<4;i++){
        gym_state_c_vect.push_back(get_random_gym_state());
       
    }

    //clients sending data 
    for(int i=0;i<4;i++){
        bool succ = client_list[i]->send(gym_state_c_vect[i]);
        clients_succ.push_back(succ);
    }

    // check data recived on the client side
    for(int i=0;i<4;i++){
        bool client_successfully_send_state = clients_succ[i];
        while(client_successfully_send_state){
            vector <boost::uuids::uuid> clients_id = server->getClientsInQueue();
            if (clients_id.size()==1){
                boost::uuids::uuid & id = clients_id[0];
                gym_state_s = server->getGymStateforId(id);
                server->removeStateFromBuffer(id);

                server->remove_client_from_queue(id);

                EXPECT_EQ(id,client_list[i]->get_id());
                
                run_gym_state_test(502,gym_state_c_vect[i],gym_state_s);
                break;
            }

        }
    
    }
    std::cout << green << "Test 7 Passed"  << def <<std::endl;
    delete server;
    for(Client_MQ* c:client_list){
        delete c;
    }
    return true;

}

//##################Client #######################

bool Test8(){
    /*
    Testing the client recives right data from server 
    */

    Server_MQ* server = new Server_MQ();
    Client_MQ* client = new Client_MQ();
    

    server->init();
    server->create_run_thread();

    client->init();
    client->create_run_thread();

    GymworldState gym_state_s;
    GhostWorldState ghost_state_s;

    GymworldState gym_state_c;
    GhostWorldState ghost_state_c;

    bool client_successfully_send_state;
    bool server_successfully_send_state;
    //client send Gym state to server
    gym_state_c = get_random_gym_state();
    client_successfully_send_state = client->send(gym_state_c);

    int n =1;
    while(client_successfully_send_state){
        vector <boost::uuids::uuid> clients_id = server->getClientsInQueue();
        if (clients_id.size()==1){
            boost::uuids::uuid & id = clients_id[0];
            server->removeStateFromBuffer(id);
            
            
          
            ghost_state_s = get_random_ghost_state(n);
  
       
            // cout<<"generated random ghost state::"<<endl;
            // print_ghost_state(ghost_state_s);
            
            server_successfully_send_state = server->send(id,ghost_state_s);
            server->remove_client_from_queue(id);
            
            break;
        }

    }
    
    while(server_successfully_send_state){
        
        sleep_until(system_clock::now() + 1s);
        ghost_state_c = client-> getGhostStateforClient();
                
        // cout<<"ghost_state_s::"<<endl;
        // print_ghost_state(ghost_state_s);
        // cout<<"ghost_state_c::"<<endl;
        // print_ghost_state(ghost_state_c);
        run_ghost_state_test(__LINE__,ghost_state_c,ghost_state_s);
        break;

    }
    

    std::cout << green << "Test 8 Passed"  << def <<std::endl;
    delete client;
    delete server;
    return true;

}

bool Test9(){
    /*
    Testing the clients recives right data from server 
    */
    vector <bool> clients_succ;
    vector <bool> server_succ;

    bool client_successfully_send_state;
    bool server_successfully_send_state;

    Server_MQ* server = new Server_MQ();
    
    Client_MQ* client_1 = new Client_MQ();
    Client_MQ* client_2 = new Client_MQ();
    Client_MQ* client_3 = new Client_MQ();
    Client_MQ* client_4 = new Client_MQ();
    vector <Client_MQ*> client_list{client_1,client_2,client_3,client_4};
    
    server->init();
    server->create_run_thread();

    for(Client_MQ* c:client_list){
        c->init();
        c->create_run_thread();
    }
    cout<<"Test9::1"<<endl;
    
    // initalizing 
    GymworldState gym_state_s;
    GhostWorldState ghost_state_s;

    GymworldState gym_state_c;
    GhostWorldState ghost_state_c;

    vector <GhostWorldState> ghost_state_s_vect;
    vector <GymworldState> gym_state_c_vect;

    //intializing clients state randomly
    for(int i=0;i<4;i++){
        gym_state_c_vect.push_back(get_random_gym_state());
       
    }
    cout<<"Test9::2"<<endl;
    //clients sending data 
    for(int i=0;i<4;i++){
        bool succ = client_list[i]->send(gym_state_c_vect[i]);
        clients_succ.push_back(succ);
    }

    cout<<"Test9::3"<<endl;
    // If data is successfuly sent to sever we send back server state back to clients
    for(int i=0;i<4;i++){
        bool client_successfully_send_state = clients_succ[i];
        while(client_successfully_send_state){
            vector <boost::uuids::uuid> clients_id = server->getClientsInQueue();
            if (clients_id.size()==1){
                boost::uuids::uuid & id = clients_id[0];
                server->removeStateFromBuffer(id);
                
                ghost_state_s = get_random_ghost_state(i);

                ghost_state_s_vect.push_back(ghost_state_s);
                bool succ = server->send(id,ghost_state_s);
                server_succ.push_back(succ);

                server->remove_client_from_queue(id);
                break;
            }

        }
    
    }
    cout<<"Test9::4"<<endl;
    // here we check the data send by the server is correctly recived by the client (eaxtly the same)
  
    for(int i=0;i<4;i++){
        bool server_successfully_send_state = server_succ[i];
        ghost_state_s = ghost_state_s_vect[i];
        while(server_successfully_send_state){
          sleep_until(system_clock::now() + 1s);
          ghost_state_c = client_list[i]-> getGhostStateforClient();
        // cout<<"ghost_state_c::"<<endl;
        //   print_ghost_state(ghost_state_c);
        //   cout<<"ghost_state_s::"<<endl;
        //   print_ghost_state(ghost_state_s);
          run_ghost_state_test(680,ghost_state_c,ghost_state_s);
          break;

        }
    }

    cout<<"Test9::5"<<endl;

    std::cout << green << "Test 9 Passed"  << def <<std::endl;
    delete server;
    for(Client_MQ* c:client_list){
        delete c;
    }
    return true;

}
//#################Time step ####################
bool Test10(){
    /*
    Testing the clients  time step
    make sure client time step (reuqested_timestep )is sent to server and back
    ghost state that recived should have the time step to gym state sent
    */

    Server_MQ* server = new Server_MQ();
    Client_MQ* client = new Client_MQ();
    
    server->init();
    server->create_run_thread();

    client->init();
    client->create_run_thread();


    GymworldState gym_state_s;
    GhostWorldState ghost_state_s;
    string reuqested_timestep_reciveved_by_server;
    string reuqested_timestep_sent_by_server;

    GymworldState gym_state_c;
    GhostWorldState ghost_state_c;

    bool client_successfully_send_state;
    bool server_successfully_send_state;

    //client send Gym state to server
    gym_state_c = get_random_gym_state();
    client_successfully_send_state = client->send(gym_state_c);

    int n =1;
    cout<<"Test10::1"<<endl;

    while(client_successfully_send_state){
        vector <boost::uuids::uuid> clients_id = server->getClientsInQueue();
        cout<<"Test10:2"<<endl;
        if (clients_id.size()==1){
            boost::uuids::uuid & id = clients_id[0];
            gym_state_s = server->getGymStateforId(id);
            reuqested_timestep_reciveved_by_server = gym_state_s.key.request_timestep;
          
            
            ghost_state_s = get_random_ghost_state(n);
  
            service_keys key = server->getKeyId(id);
            cout<<"key::request_timestep"<<key.request_timestep<<endl;
            server_successfully_send_state=server->send(key,ghost_state_s);
            server->removeStateFromBuffer(id);
            server->remove_client_from_queue(id);
             cout<<"Test10:3"<<endl;
            
            break;
        }

    }
    
    while(server_successfully_send_state){
        cout<<"Test10:4"<<endl;
        sleep_until(system_clock::now() + 1s);
        ghost_state_c = client-> getGhostStateforClient();
        reuqested_timestep_sent_by_server =  ghost_state_c.key.request_timestep;
        cout<<"ghost_state_c::served_timestep::"<<ghost_state_c.key.served_timestep<<endl;
        run_ghost_state_test(__LINE__,ghost_state_c,ghost_state_s);
        break;

    }
    cout<<"Test10:5"<<endl;
    cout<<"reuqested_timestep_reciveved_by_server::"<<reuqested_timestep_reciveved_by_server<<endl;
    cout<<"reuqested_timestep_sent_by_server::"<<reuqested_timestep_sent_by_server<<endl;
    EXPECT_EQ_STR(reuqested_timestep_reciveved_by_server,reuqested_timestep_sent_by_server);
    
    std::cout << green << "Test 10 Passed"  << def <<std::endl;

    delete client;
    delete server;

    return true;

}
int main(void) {

    vector<BoolFunc>  test_list ={
         Test1
        ,Test2
        ,Test3
        ,Test4
        ,Test5
        ,Test6
        ,Test7
        ,Test8
        ,Test9
        ,Test10
    };
    bool succ = false; 
    for(int i=0;i<test_list.size();i++){
        if (i==0){
            succ = test_list[i]();
        }else{
            if (succ) test_list[i]();
        }
    }

    return 0;
}