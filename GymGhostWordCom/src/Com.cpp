
#include "Com/Com.h"



using namespace std;

string str(const char *str){
    string s(str);
    return s;
}

string uuid_s(boost::uuids::uuid client_id){

    // cout<<"uuid:: "<<client_id<<endl;
    // cout<<"uuid::lexical_cast:: "<<boost::lexical_cast<std::string>(client_id)<<endl;
    return  boost::lexical_cast<std::string>(client_id);
}


GymworldState Parser::parseGymState(char data[]){
    GymworldState gymState;
    Json::Value v_json;
    Json::Reader reader;

    
    reader.parse(data,v_json);
    //key
    // cout<<"gymState.key.id:: "<<v_json["key"] <<endl;
    // cout<<"gymState.key.id::type:: "<<typeid(v_json["key"].asString()).name() <<endl;
    gymState.key.id =boost::lexical_cast<boost::uuids::uuid >(v_json["key"].asString());
   
    //ff   
    gymState.ff.pos.x = v_json["ff"]["pos"][0].asDouble();
    gymState.ff.pos.y = v_json["ff"]["pos"][1].asDouble();
    gymState.ff.pos.z = v_json["ff"]["pos"][2].asDouble();
    gymState.ff.orn.r = v_json["ff"]["orn"][0].asDouble();
    gymState.ff.orn.p = v_json["ff"]["orn"][1].asDouble();
    gymState.ff.orn.y = v_json["ff"]["orn"][2].asDouble();
    //mf
    gymState.mf.pos.x = v_json["mf"]["pos"][0].asDouble();
    gymState.mf.pos.y = v_json["mf"]["pos"][1].asDouble();
    gymState.mf.pos.z = v_json["mf"]["pos"][2].asDouble();
    gymState.mf.orn.r = v_json["mf"]["orn"][0].asDouble();
    gymState.mf.orn.p = v_json["mf"]["orn"][1].asDouble();
    gymState.mf.orn.y = v_json["mf"]["orn"][2].asDouble();
    //rf
    gymState.rf.pos.x = v_json["rf"]["pos"][0].asDouble();
    gymState.rf.pos.y = v_json["rf"]["pos"][1].asDouble();
    gymState.rf.pos.z = v_json["rf"]["pos"][2].asDouble();
    gymState.rf.orn.r = v_json["rf"]["orn"][0].asDouble();
    gymState.rf.orn.p = v_json["rf"]["orn"][1].asDouble();
    gymState.rf.orn.y = v_json["rf"]["orn"][2].asDouble();
    //th
    gymState.th.pos.x = v_json["th"]["pos"][0].asDouble();
    gymState.th.pos.y = v_json["th"]["pos"][1].asDouble();
    gymState.th.pos.z = v_json["th"]["pos"][2].asDouble();
    gymState.th.orn.r = v_json["th"]["orn"][0].asDouble();
    gymState.th.orn.p = v_json["th"]["orn"][1].asDouble();
    gymState.th.orn.y = v_json["th"]["orn"][2].asDouble();
    //cube
    gymState.cube.pos.x = v_json["cube"]["pos"][0].asDouble();
    gymState.cube.pos.y = v_json["cube"]["pos"][1].asDouble();
    gymState.cube.pos.z = v_json["cube"]["pos"][2].asDouble();
    gymState.cube.orn.r = v_json["cube"]["orn"][0].asDouble();
    gymState.cube.orn.p = v_json["cube"]["orn"][1].asDouble();
    gymState.cube.orn.y = v_json["cube"]["orn"][2].asDouble();

    return gymState;
}

GhostWorldState Parser::parseGhostState(char data[]){
    GhostWorldState ghostState;
    Json::Value v_json;
    Json::Reader reader;

    
    reader.parse(data,v_json);
    //key
    ghostState.key.id =boost::lexical_cast<boost::uuids::uuid >(v_json["key"].asString());
    //ff   
    ghostState.ff = v_json["ff"].asInt();
    ghostState.mf = v_json["mf"].asInt();
    ghostState.rf = v_json["rf"].asInt();
    ghostState.th = v_json["th"].asInt();

    return ghostState;
}

string Searlalize::gymState(GymworldState gym_State){
    
    char data[BUFFER_SIZE];
 
    sprintf(data, GymStateFormat,uuid_s(gym_State.key.id).c_str(),
                                 gym_State.ff.pos.x,gym_State.ff.pos.y,gym_State.ff.pos.z,gym_State.ff.orn.r,gym_State.ff.orn.p,gym_State.ff.orn.y,
                                 gym_State.mf.pos.x,gym_State.mf.pos.y,gym_State.mf.pos.z,gym_State.mf.orn.r,gym_State.mf.orn.p,gym_State.mf.orn.y,
                                 gym_State.rf.pos.x,gym_State.rf.pos.y,gym_State.rf.pos.z,gym_State.rf.orn.r,gym_State.rf.orn.p,gym_State.rf.orn.y,
                                 gym_State.th.pos.x,gym_State.th.pos.y,gym_State.th.pos.z,gym_State.th.orn.r,gym_State.th.orn.p,gym_State.th.orn.y,
                                 gym_State.cube.pos.x,gym_State.cube.pos.y,gym_State.cube.pos.z,gym_State.cube.orn.r,gym_State.cube.orn.p,gym_State.cube.orn.y
    
     );
    // cout<<"serialize::gymState::data:: "<<data<<endl;



    return string(data);
}

string Searlalize::ghostState(GhostWorldState ghost_State){
    char data[BUFFER_SIZE];
    sprintf(data, GhostSateFormat,uuid_s(ghost_State.key.id).c_str(),
                                  int(ghost_State.ff),
                                  int(ghost_State.mf),
                                  int(ghost_State.rf),
                                  int(ghost_State.th)
            );

    string data_s =string(data);

    return string(data);
}

string Searlalize::uuid_s(boost::uuids::uuid client_id){

    // cout<<"uuid:: "<<client_id<<endl;
    // cout<<"uuid::lexical_cast:: "<<boost::lexical_cast<std::string>(client_id)<<endl;
    return  boost::lexical_cast<std::string>(client_id);
}
int Searlalize::getGymStateBufferSize(){
    return this->buffer_length;
}


//****************Masage Queue**************************
Server_MQ::Server_MQ(){

    
    // Create basic file logger (not rotated)
    #if LOGFLAG
    auto my_logger = spdlog::basic_logger_mt("basic_logger", "logs/server_MQ.txt");
    spdlog::set_default_logger(my_logger);
    spdlog::flush_on(spdlog::level::info);

    spdlog::info("Server::"+str(__FUNCTION__));
    #endif

    /*To set msgQ attributes*/
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    LOG("Server::"+str(__FUNCTION__)+"::end");

}

void Server_MQ::init(){
   
    LOG("Server::"+str(__FUNCTION__));
    
    mq_unlink(MSG_Q_Name_GYM);
    if ((msgq_fd  = mq_open (MSG_Q_Name_GYM, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        printf ("Server: mq_open failed, errno = %d", errno);
        exit (1);
    }
    LOG("Server::"+str(__FUNCTION__)+"::end");
    
}
void Server_MQ::run(){
 
    while(1){
        LOG("Server::"+str(__FUNCTION__));
        FD_ZERO(&readfds);
        FD_SET( msgq_fd, &readfds);
        // cout<<"Reciever blocked on select()....\r";
        // cout<<"msgq_fd + 1::"<<msgq_fd + 1<<endl;
        select(msgq_fd + 1, &readfds, NULL, NULL, NULL);
        // cout<<"msgq_fd + 1::"<<msgq_fd + 1<<endl;
        if(FD_ISSET(msgq_fd, &readfds)){
            \
            
            memset(buffer, 0, MSG_BUFFER_SIZE);
            if (mq_receive (msgq_fd, buffer, MSG_BUFFER_SIZE, NULL) == -1) {
                printf ("mq_receive error, errno = %d\n", errno);
                exit (1);
            }
            // printf("Msg recieved from Queue = %s\n", buffer);
            /* Recive Gym State*/
            GymworldState gym_State;
            
            char data_c[BUFFER_SIZE];
            // cout<<"buffer::"<<buffer<<endl;
            memcpy(&data_c, buffer,BUFFER_SIZE);
            //parse data
            gym_State = parser->parseGymState(data_c);
            // cout<<"gymstate::ff::pos::x"<<gym_State.ff.pos.x<<endl;

            add_client_to_queue(gym_State.key.id);
            add_client_state_to_buffer(gym_State.key.id,gym_State);
            LOG("Server::"+str(__FUNCTION__)+"::end");
        }
    }
    
}

bool Server_MQ::create_run_thread(){
    LOG("Server::"+str(__FUNCTION__));
    bool success = pthread_create(&t,NULL,InternalThreadEntryFunc,this)==0;
    LOG("Server::"+str(__FUNCTION__)+"::end");
    return success;
}

void * Server_MQ::InternalThreadEntryFunc(void * This){
    LOG("Server::"+str(__FUNCTION__));
    ((Server_MQ *)This)->run(); 
    LOG("Server::"+str(__FUNCTION__)+"::end");
    return NULL;

 }


bool Server_MQ::is_there_any_client_in_queue(){
    LOG("Server::"+str(__FUNCTION__));
    pthread_mutex_lock(&this->clients_queue.lock);
    if (clients_queue.data.size() ==0){
        pthread_mutex_unlock(&this->clients_queue.lock);
        LOG("Server::"+str(__FUNCTION__)+"::end");
        return false;
    }else{
        pthread_mutex_unlock(&this->clients_queue.lock);
        LOG("Server::"+str(__FUNCTION__)+"::end");
        return true;
    }
}
bool Server_MQ::is_client_in_queue(boost::uuids::uuid client_id,bool keep_mutex_locked){
    LOG("Server::"+str(__FUNCTION__));
    // cout<<"I am here::is_client_in_queue"<<endl; 
    // pthread_mutex_lock(&this->clients_queue.lock);
    if (clients_queue.data.size() ==0){
        // cout<<"I am here::is_client_in_queue::false::1"<<endl; 
        if (!keep_mutex_locked) pthread_mutex_unlock(&this->clients_queue.lock);
        LOG("Server::"+str(__FUNCTION__)+"::end");
       
        return false;
    }else{
        for(int i = 0; i < clients_queue.data.size(); ++i){
            string str1 = boost::lexical_cast<std::string>(clients_queue.data[i]);
            string str2=boost::lexical_cast<std::string>(client_id);
            bool same = !strcmp(str1.c_str(), str2.c_str());
            // cout<<"clients_queue.data[i]::"<<boost::lexical_cast<std::string>(clients_queue.data[i])<<endl;
            // cout<<"client_id::"<<boost::lexical_cast<std::string>(client_id)<<endl;
            // cout<<"same::"<< std::boolalpha << same <<endl;
            if (same){

                // cout<<"I am here::is_client_in_queue::True"<<endl;
                if (!keep_mutex_locked) pthread_mutex_unlock(&this->clients_queue.lock);
                LOG("Server::"+str(__FUNCTION__)+"::end");
                return true;
            }
                
        }
        // cout<<"I am here::is_client_in_queue::false::2"<<endl;
        if (!keep_mutex_locked) pthread_mutex_unlock(&this->clients_queue.lock);
        LOG("Server::"+str(__FUNCTION__)+"::end");
        return false;
   
    }
}

bool Server_MQ::is_gymState_in_state_buffer(boost::uuids::uuid client_id){
    LOG("Server::"+str(__FUNCTION__));
    pthread_mutex_lock(&state_buffer_mutex);
    if  (state_buffer.size()==0){
        pthread_mutex_unlock(&state_buffer_mutex);
        LOG("Server::"+str(__FUNCTION__)+"::end");
        return false;
    }else{
        for(int i = 0; i < state_buffer.size(); ++i){
            if (state_buffer[i].key.id==client_id){
                pthread_mutex_unlock(&state_buffer_mutex);
                LOG("Server::"+str(__FUNCTION__)+"::end");
                return true;

            }
        }
        pthread_mutex_unlock(&state_buffer_mutex);
        LOG("Server::"+str(__FUNCTION__)+"::end");
        return false;
    }
        
    
}
void Server_MQ::add_client_to_queue(boost::uuids::uuid client_id){
    LOG("Server::"+str(__FUNCTION__));
    //add client to queue if it is not already in queue 
    pthread_mutex_lock(&this->clients_queue.lock);
    // cout<<"I am here::add_client_to_queue"<<endl; 
    bool flag=!is_client_in_queue(client_id,true);
    if (flag){
        // pthread_mutex_lock(&this->clients_queue.lock);
        // cout<<"add_client_to_queue::locking the resource"<<endl;
        clients_queue.data.push_back(client_id);
       
        pthread_mutex_unlock(&this->clients_queue.lock);
        // cout<<"add_client_to_queue::unlocking the resource"<<endl;
        LOG("Server::"+str(__FUNCTION__)+"::end");
        return;
    }
    pthread_mutex_unlock(&this->clients_queue.lock);
    LOG("Server::"+str(__FUNCTION__)+"::end");
   
}

void Server_MQ::add_client_state_to_buffer(boost::uuids::uuid client_id,GymworldState gymState){
    //update state if already in buffer or add state if not  
      LOG("Server::"+str(__FUNCTION__));
    
    // cout<<"i am here::add_client_state_to_buffer"<<endl;
    if (is_gymState_in_state_buffer(client_id)){
        pthread_mutex_lock(&state_buffer_mutex);
        for(int i = 0; i < state_buffer.size(); ++i){
            if (state_buffer[i].key.id==client_id){
                //update state
                state_buffer[i] =gymState;
                pthread_mutex_unlock(&state_buffer_mutex);
                LOG("Server::"+str(__FUNCTION__)+"::end");
                return;

            }
        }
    }else{
        //add state
        pthread_mutex_lock(&state_buffer_mutex);
        state_buffer.push_back(gymState);
    }
    pthread_mutex_unlock(&state_buffer_mutex);
    LOG("Server::"+str(__FUNCTION__)+"::end");
    

}
void Server_MQ::remove_client_from_queue(boost::uuids::uuid client_id){
    cout<<"Server_MQ::remove_client_from_queue:: called"<<endl;
    LOG("Server::"+str(__FUNCTION__));
   
    pthread_mutex_lock(&this->clients_queue.lock);
    for(int i = 0; i < clients_queue.data.size(); ++i){
        if (clients_queue.data[i]==client_id){
            
            clients_queue.data.erase(clients_queue.data.begin()+i);
            pthread_mutex_unlock(&this->clients_queue.lock);
            LOG("Server::"+str(__FUNCTION__)+"::end");
            cout<<"Server_MQ::remove_client_from_queue:: ended"<<endl;
            return;
            
        }
    }
    pthread_mutex_unlock(&this->clients_queue.lock);
    cout<<"Server_MQ::remove_client_from_queue:: ended"<<endl;
    LOG("Server::"+str(__FUNCTION__)+"::end");
    

}
void Server_MQ::removeStateFromBuffer(boost::uuids::uuid client_id){
    cout<<"Server_MQ::removeStateFromBuffer:: called"<<endl;
    LOG("Server::"+str(__FUNCTION__));  
    pthread_mutex_lock(&state_buffer_mutex);   
    for(int i = 0; i < state_buffer.size(); ++i){
        if (state_buffer[i].key.id==client_id){
            //remove state
            
            state_buffer.erase(state_buffer.begin()+i);
            cout<<"Server_MQ::removeStateFromBuffer:: ended"<<endl;
            break;
        }

    }
    pthread_mutex_unlock(&state_buffer_mutex);
    cout<<"Server_MQ::removeStateFromBuffer:: ended"<<endl;
    LOG("Server::"+str(__FUNCTION__)+"::end");

}
vector <boost::uuids::uuid> Server_MQ::getClientsInQueue(){
    LOG("Server::"+str(__FUNCTION__));
    cout<<"Server_MQ::getClientsInQueue:: called"<<endl;
    vector <boost::uuids::uuid> clients_queue_ids;
    pthread_mutex_lock(&this->clients_queue.lock);
    // cout<<"getClientsInQueue::locking the resource"<<endl;
    for(int i = 0; i < clients_queue.data.size(); ++i){
        
     
        clients_queue_ids.push_back(clients_queue.data[i]);

    }
    // cout<<"getClientsInQueue::unlocking the resource"<<endl;
    pthread_mutex_unlock(&this->clients_queue.lock);
    cout<<"Server_MQ::getClientsInQueue:: edned"<<endl;
    LOG("Server::"+str(__FUNCTION__)+"::end");
    return clients_queue_ids;
}

GymworldState Server_MQ::getGymStateforId(boost::uuids::uuid client_id){
    LOG("Server::"+str(__FUNCTION__));
    pthread_mutex_lock(&state_buffer_mutex);
    // cout<<"i am here::getGymStateforId"<<endl;
    for(int i = 0; i < this->state_buffer.size(); ++i){
        // cout<<"i am here::getGymStateforId::I am insdie loop"<<endl;
        if (this->state_buffer[i].key.id==client_id){
            GymworldState state = this->state_buffer[i];
            pthread_mutex_unlock(&state_buffer_mutex);
            LOG("Server::"+str(__FUNCTION__)+"::end");
            return state;
        }

    }
    pthread_mutex_unlock(&state_buffer_mutex);
    LOG("Server::"+str(__FUNCTION__)+"::end");
   
}

bool Server_MQ::send(boost::uuids::uuid client_id,GhostWorldState ghostState){
    LOG("Server::"+str(__FUNCTION__));
    cout<<"Server_MQ::send:: called"<<endl;
    string client_mqName = MSG_Q_Name_GHOST+boost::uuids::to_string(client_id);
    ghostState.key.id = client_id;
    ghostState.key.mqName = client_mqName;

    // cout<<"server::send::q_name:: "<<client_mqName<<endl;

    milliseconds ms = duration_cast< milliseconds >(
    system_clock::now().time_since_epoch());
    ghostState.key.timestep =ms;


    //preparing the data
    char data[BUFFER_SIZE];
    strcpy(data,searlalizer->ghostState(ghostState).c_str());

    //Creating a message queue
    if ((recvr_msgq_fd  = mq_open (client_mqName.c_str(), O_WRONLY | O_CREAT, 0, 0)) == -1) {
        printf ("Server: mq_open failed, errno = %d", errno);
        exit (1);
    }
    
    //Adding message to the Queue
    if (mq_send (recvr_msgq_fd, data, strlen (data) + 1, 0) == -1) {
        perror ("Client: Not able to send message to server");
        exit (1);
    }
    mq_close(recvr_msgq_fd);
    cout<<"Server_MQ::send:: ended"<<endl;
    LOG("Server::"+str(__FUNCTION__)+"::end");
    return true;

}

Client_MQ::Client_MQ(){

    // genrate an id for client
    this->id = generate_UUID_for_this_client();
    string id_str = boost::lexical_cast<std::string>(this->id);
    // Create basic file logger (not rotated)
    auto my_logger = spdlog::basic_logger_mt("basic_logger", "logs/client_MQ"+id_str+".txt");
    spdlog::set_default_logger(my_logger);
    spdlog::flush_on(spdlog::level::info);

    LOG("Client::"+str(__FUNCTION__));
    //Creating a Message Queue In order to listen to messagees coming form server

      //Creating a Message Queue In order to listen to messagees coming form server
    this->mqName = generate_mqName_for_this_client(this->id);

    /*To set msgQ attributes*/
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    LOG("Client::"+str(__FUNCTION__)+"::end");

}
void Client_MQ::init(){

    LOG("Client::"+str(__FUNCTION__));
    // cout<<"client::q_name:: "<<this->mqName.c_str()<<endl;
    if ((msgq_fd  = mq_open (this->mqName.c_str(), O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        printf ("Client: mq_open failed, errno = %d", errno);
        exit (1);
    }
    LOG("Client::"+str(__FUNCTION__)+"::end");
    
}
void Client_MQ::run(){

  
    while(1){
        LOG("Client::"+str(__FUNCTION__));
        FD_ZERO(&readfds);
        FD_SET( msgq_fd, &readfds);
        // printf("Reciever blocked on select()....\n");
        // cout<<"msgq_fd + 1::"<<msgq_fd + 1<<endl;
        select(msgq_fd + 1, &readfds, NULL, NULL, NULL);
        if(FD_ISSET(msgq_fd, &readfds)){
            \
            
            memset(buffer, 0, MSG_BUFFER_SIZE);
            if (mq_receive (msgq_fd, buffer, MSG_BUFFER_SIZE, NULL) == -1) {
                printf ("mq_receive error, errno = %d\n", errno);
                exit (1);
            }
            // printf("Msg recieved from Queue = %s\n", buffer);
            /* Recive Ghost State*/
            GhostWorldState ghost_State;
            
            char data_c[BUFFER_SIZE];
            // cout<<"buffer::"<<buffer<<endl;
            memcpy(&data_c, buffer,BUFFER_SIZE);
            //parse data
            ghost_State = parser->parseGhostState(data_c);
            // cout<<"ghost_State::ff::"<<ghost_State.ff<<endl;

            //update Ghost state
            updateState(ghost_State);
        }
        LOG("Client::"+str(__FUNCTION__)+"::end");
    }
    
}
bool Client_MQ::send(GymworldState gymState){
    LOG("Client::"+str(__FUNCTION__));
    gymState.key.id = this->id;
    gymState.key.mqName = this->mqName;
    //time step
    milliseconds ms = duration_cast< milliseconds >(
    system_clock::now().time_since_epoch());
    gymState.key.timestep =ms;


    //preparing the data
    char data[BUFFER_SIZE];
    strcpy(data,searlalizer->gymState(gymState).c_str());

    //Creating a message queue
    if ((recvr_msgq_fd  = mq_open (MSG_Q_Name_GYM, O_WRONLY | O_CREAT, 0, 0)) == -1) {
        printf ("Client: mq_open failed, errno = %d", errno);
        exit (1);
    }
    
    //Adding message to the Queue
    if (mq_send (recvr_msgq_fd, data, strlen (data) + 1, 0) == -1) {
        perror ("Client: Not able to send message to server");
        exit (1);
    }
    mq_close(recvr_msgq_fd);
    LOG("Client::"+str(__FUNCTION__)+"::end");
    return true;
}
bool Client_MQ::create_run_thread(){
    LOG("Client::"+str(__FUNCTION__));
    bool success = pthread_create(&t,NULL,InternalThreadEntryFunc,this)==0;
    LOG("Client::"+str(__FUNCTION__)+"::end");
    return success;
}

void * Client_MQ::InternalThreadEntryFunc(void * This){
    LOG("Client::"+str(__FUNCTION__));
    ((Client_MQ *)This)->run(); 
    LOG("Client::"+str(__FUNCTION__)+"::end");
    return NULL;

}

string Client_MQ::generate_mqName_for_this_client(boost::uuids::uuid id){
    //To make sure message queue name is unique we use cliend id as part of the name
    LOG("Client::"+str(__FUNCTION__));
    return  MSG_Q_Name_GHOST+ boost::uuids::to_string(id);
}
boost::uuids::uuid Client_MQ::generate_UUID_for_this_client(){
    LOG("Client::"+str(__FUNCTION__));
    return   boost::uuids::random_generator()();
}
void Client_MQ::updateState(GhostWorldState ghost_State){
    LOG("Client::"+str(__FUNCTION__));
    pthread_mutex_lock(&state_mutex);
    this->ghostState = ghost_State;
    // cout<<"Client_MQ::updateState::ghost_State::id"<<boost::uuids::to_string(ghost_State.key.id)<<endl;
    bool id_is_a_match =boost::uuids::to_string(ghost_State.key.id) == boost::uuids::to_string(this->id);
    // cout<<"id_is_a_match:: "<<std::boolalpha <<  id_is_a_match<<endl;
    pthread_mutex_unlock(&state_mutex);
    LOG("Client::"+str(__FUNCTION__)+"::end");
}
GhostWorldState Client_MQ::getGhostStateforClient(){
  
    LOG("Client::"+str(__FUNCTION__));
    //deque new state form message queue
    pthread_mutex_lock(&state_mutex);
    GhostWorldState ghost_state = this->ghostState;
    pthread_mutex_unlock(&state_mutex);
    LOG("Client::"+str(__FUNCTION__)+"::end");
    return ghost_state;
    
 }
