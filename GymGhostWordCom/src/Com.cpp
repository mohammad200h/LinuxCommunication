
#include "Com/Com.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h" // support for basic file logging
#include "spdlog/sinks/rotating_file_sink.h" // support for rotating file logging

using namespace std;

string str(const char *str){
    string s(str);
    return s;
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


//****************Unix Domain Sockets**************************


Server::Server(){
 
}

 void Server::intitiaze_monitor_fd_set(){

    int i = 0;
    for(; i < MAX_CLIENT_SUPPORTED; i++)
        monitored_fd_set[i] = -1;
}

/*Add a new FD to the monitored_fd_set array*/
void Server::add_to_monitored_fd_set(int skt_fd){

    int i = 0;
    for(; i < MAX_CLIENT_SUPPORTED; i++){

        if(monitored_fd_set[i] != -1)
            continue;
        monitored_fd_set[i] = skt_fd;
        break;
    }
}

/*Remove the FD from monitored_fd_set array*/
void Server::remove_from_monitored_fd_set(int skt_fd){

    int i = 0;
    for(; i < MAX_CLIENT_SUPPORTED; i++){

        if(monitored_fd_set[i] != skt_fd)
            continue;

        monitored_fd_set[i] = -1;
        break;
    }
}

/* Clone all the FDs in monitored_fd_set array into 
 * fd_set Data structure*/
void Server::refresh_fd_set(fd_set *fd_set_ptr){

    FD_ZERO(fd_set_ptr);
    int i = 0;
    for(; i < MAX_CLIENT_SUPPORTED; i++){
        if(monitored_fd_set[i] != -1){
            FD_SET(monitored_fd_set[i], fd_set_ptr);
        }
    }
}

/*Get the numerical max value among all FDs which server
 * is monitoring*/

int Server::get_max_fd(){

    int i = 0;
    int max = -1;

    for(; i < MAX_CLIENT_SUPPORTED; i++){
        if(monitored_fd_set[i] > max)
            max = monitored_fd_set[i];
    }

    return max;
}

bool Server::create_run_thread(){
    bool success = pthread_create(&t,NULL,InternalThreadEntryFunc,this)==0;

    return success;
}

void * Server::InternalThreadEntryFunc(void * This){
     ((Server *)This)->run(); 
     return NULL;

 }

void Server::init(){

    intitiaze_monitor_fd_set();
    add_to_monitored_fd_set(0);
    
    /*In case the program exited inadvertently on the last run,
     *remove the socket.
     **/

    unlink(SOCKET_NAME);

    /* Create Master socket. */

    /*SOCK_DGRAM for Datagram based communication*/
    connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    // printf("connection_socket:: %d",connection_socket);
    cout<<"connection_socket::"<<connection_socket<<endl;
    if (connection_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    printf("Master socket created\n");

    /*initialize*/
    memset(&name, 0, sizeof(struct sockaddr_un));

    /*Specify the socket Cridentials*/
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    /* Bind socket to socket name.*/
    /* Purpose of bind() system call is that application() dictate the underlying 
     * operating system the criteria of recieving the data. Here, bind() system call
     * is telling the OS that if sender process sends the data destined to socket "/tmp/DemoSocket", 
     * then such data needs to be delivered to this server process (the server process)*/
    ret = bind(connection_socket, (const struct sockaddr *) &name,
            sizeof(struct sockaddr_un));

    if (ret == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    printf("bind() call succeed\n");
    /*
     * Prepare for accepting connections. The backlog size is set
     * to 20. So while one request is being processed other requests
     * can be waiting.
     * */

    ret = listen(connection_socket, 20);
    if (ret == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /*Add master socket to Monitored set of FDs*/
    add_to_monitored_fd_set(connection_socket);

    /* This is the main loop for handling connections. */
    /*ll Server process usually runs 24 x 7. Good Servers should always up
     * and running and shold never go down. Have you ever seen Facebook Or Google
     * page failed to load ??*/
    
}

void Server::run(){
    

    while(!terminate_server_flag) {
       
        refresh_fd_set(&readfds); /*Copy the entire monitored FDs to readfds*/
        /* Wait for incoming connection. */
        printf("Waiting on select() sys call\n");
        serverIsReady = true;
        /* Call the select system call, server process blocks here. 
         * Linux OS keeps this process blocked untill the connection initiation request Or 
         * data requests arrives on any of the file Drscriptors in the 'readfds' set*/

        select(get_max_fd() + 1, &readfds, NULL, NULL, NULL);

        if(FD_ISSET(connection_socket, &readfds)){

            /*Data arrives on Master socket only when new client connects with the server (that is, 'connect' call is invoked on client side)*/
            printf("New connection recieved recvd, accept the connection\n");

            data_socket = accept(connection_socket, NULL, NULL);

            if (data_socket == -1) {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            printf("Connection accepted from client::%d\n",data_socket);

            add_to_monitored_fd_set(data_socket);
        }
        else if(FD_ISSET(0, &readfds)){
            memset(buffer, 0, BUFFER_SIZE);
            ret = read(0, buffer, BUFFER_SIZE);
            printf("Input read from console : %s\n", buffer);
        }
        else /* Data srrives on some other client FD*/
        {
            /*Find the client which has send us the data request*/
            i = 0, comm_socket_fd = -1;
            for(; i < MAX_CLIENT_SUPPORTED; i++){

                if(FD_ISSET(monitored_fd_set[i], &readfds) && !is_client_in_queue(monitored_fd_set[i])){
                    comm_socket_fd = monitored_fd_set[i];

                    /*Prepare the buffer to recv the data*/
                    memset(buffer, 0, BUFFER_SIZE);

                    /* Wait for next data packet. */
                    /*Server is blocked here. Waiting for the data to arrive from client
                     * 'read' is a blocking system call*/
                    printf("Waiting for data from the client:: %d\n",comm_socket_fd);
                    
                    ret = read(comm_socket_fd,buffer,BUFFER_SIZE);

                    if (ret == -1) {
                        cout<<"read error"<<endl;
                        perror("read");
                        exit(EXIT_FAILURE);
                    }

                    /* Recive Gym State*/
                    GymworldState gym_State;
                    
                    char data_c[BUFFER_SIZE];
                    // cout<<"buffer::"<<buffer<<endl;
                    memcpy(&data_c, buffer,BUFFER_SIZE);
                    //parse data
                    gym_State = parser->parseGymState(data_c);
                    //Assign socket to Gymstate key
                    gym_State.key.socket = comm_socket_fd;
                    add_client_to_queue(gym_State.key.id,gym_State.key.socket);
                    add_client_state_to_buffer(gym_State.key.id,gym_State);
                    // printf("client with socket %d was adeed to queue",comm_socket_fd);
                    cout<<"client with socket" <<comm_socket_fd<< "was adeed to queue"<<endl;
                }
            }
        }
    

    } /*go to select() and block*/
    cout<<"outside the loop"<<endl;
    /*close the master socket*/
    close(connection_socket);
    remove_from_monitored_fd_set(connection_socket);
    printf("connection closed..\n");

    /* Server should release resources before getting terminated.
     * Unlink the socket. */

    unlink(SOCKET_NAME);
 


}

bool Server::send(boost::uuids::uuid client_id,GhostWorldState ghostState){

    char data[BUFFER_SIZE];
    strcpy(data,searlalizer->ghostState(ghostState).c_str());
    // memcpy(&data,searlalizer->ghostState(ghostState).c_str(),BUFFER_SIZE);
  
    int sk = get_socket_for_client(client_id);
    ret = write(sk,&data,BUFFER_SIZE);
    if (ret == -1) {
        perror("write");
        return false;
    }

    cout<<"Result has been sent to client"<<endl;
    /* Close socket. */
    

    remove_client_from_queue(ghostState.key.id);            
    return true;

}
void Server::terminate_server(){
    terminate_server_flag = true;
}
bool Server::is_there_any_client_in_queue(){
    if (clients_queue.size() ==0){
        return false;
    }else{
        return true;
    }
}
bool Server::is_client_in_queue(boost::uuids::uuid client_id){
    if (clients_queue.size() ==0){
        return false;
    }else{
        for(int i = 0; i < clients_queue.size(); ++i){
            if (clients_queue[i].first==client_id)
                return true;
        }
        return false;
    }
}
bool Server::is_client_in_queue(int client_socket){
    if (clients_queue.size() ==0){
        return false;
    }else{
        for(int i = 0; i < clients_queue.size(); ++i){
            if (clients_queue[i].second==client_socket)
                return true;
        }
        return false;
    }

}
bool Server::is_gymState_in_state_buffer(boost::uuids::uuid client_id){
    if  (state_buffer.size()==0){
        return false;
    }else{
        for(int i = 0; i < state_buffer.size(); ++i){
            if (state_buffer[i].key.id==client_id)
                return true;
        }
        return false;
    }
        
    
}
void Server::add_client_to_queue(boost::uuids::uuid client_id,int socket){
    //add client to queue if it is not already in queue   
    if (!is_client_in_queue(client_id))
        clients_queue.push_back(make_pair(client_id,socket));
   
}

void Server::add_client_state_to_buffer(boost::uuids::uuid client_id,GymworldState gymState){
    //update state if already in buffer or add state if not  
    if (is_gymState_in_state_buffer(client_id)){

        for(int i = 0; i < state_buffer.size(); ++i){
            if (state_buffer[i].key.id==client_id){
            //update state
            state_buffer[i] =gymState;
            }
        }
    }else{
         state_buffer.push_back(gymState);
    }
    

}
void Server::remove_client_from_queue(boost::uuids::uuid client_id){
    int comm_socket_fd;

    //removing client id and socket from queue    
    for(int i = 0; i < clients_queue.size(); ++i){
        if (clients_queue[i].first==client_id){
            comm_socket_fd = clients_queue[i].second;
            //colsing and removing socket 
            close(comm_socket_fd);
            remove_from_monitored_fd_set(comm_socket_fd);
            clients_queue.erase(clients_queue.begin()+i);
            
        }
    }


}
void Server::removeStateFromBuffer(boost::uuids::uuid client_id){
    for(int i = 0; i < state_buffer.size(); ++i){
        if (state_buffer[i].key.id==client_id){
            //remove state
            state_buffer.erase(state_buffer.begin()+i);
        }

    }
}
vector <boost::uuids::uuid> Server::getClientsInQueue(){
   vector <boost::uuids::uuid> clients_queue_ids;
    for(int i = 0; i < clients_queue.size(); ++i){
        clients_queue_ids.push_back(clients_queue[i].first);
    }
    return clients_queue_ids;
}

int Server::get_socket_for_client(boost::uuids::uuid client_id){
    for(int i=0;i<clients_queue.size();i++){
         if (clients_queue[i].first==client_id)
            return clients_queue[i].second;
    }
}

GymworldState Server::getGymStateforId(boost::uuids::uuid client_id){
    for(int i = 0; i < this->state_buffer.size(); ++i){
        if (this->state_buffer[i].key.id==client_id){
            //remove state
            return this->state_buffer[i];
        }

    }
}
Client::Client(){
 
    // genrate an id for client
    this->id = generate_UUID_for_this_client();


    /* Create data socket. */

    data_socket = socket(AF_UNIX, SOCK_STREAM, 0);

    if (data_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /*
     * For portability clear the whole structure, since some
     * implementations have additional (nonstandard) fields in
     * the structure.
     * */

    memset(&addr, 0, sizeof(struct sockaddr_un));

    /* Connect socket to socket address */

    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    ret = connect (data_socket, (const struct sockaddr *) &addr,
            sizeof(struct sockaddr_un));

    if (ret == -1) {
        fprintf(stderr, "The server is down.\n");
        exit(EXIT_FAILURE);
    }   
}

bool Client::send(GymworldState gymState){
    //add id to gym state
    gymState.key.id = this->id;

    char data[BUFFER_SIZE];
    strcpy(data,searlalizer->gymState(gymState).c_str());
    
    // cout<<"data::"<<data<<endl;

    int len = searlalizer->gymState(gymState).size();
    ret = write(data_socket,&data,len);
    if (ret == -1) {
        perror("write");
        return false;
    }

    /* Receive result. */
    memset(buffer, 0, BUFFER_SIZE);
    
    ret = read(data_socket, buffer, BUFFER_SIZE);
    if (ret == -1) {
        perror("read");
        return false;
    }

    /* Ensure buffer is 0-terminated. */
    buffer[BUFFER_SIZE - 1] = 0;

    /* Close socket. */
    close(data_socket);

    return true;
}

boost::uuids::uuid Client::generate_UUID_for_this_client(){
     return   boost::uuids::random_generator()();
 }

//****************Masage Queue**************************
Server_MQ::Server_MQ(){

    // Create basic file logger (not rotated)
    auto my_logger = spdlog::basic_logger_mt("basic_logger", "logs/server_MQ.txt");
    spdlog::set_default_logger(my_logger);
    spdlog::flush_on(spdlog::level::info);

    spdlog::info("Server::"+str(__FUNCTION__));

    /*To set msgQ attributes*/
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
    spdlog::info("Client::"+str(__FUNCTION__)+"::end");

}

void Server_MQ::init(){
  spdlog::info("Server::"+str(__FUNCTION__));

    mq_unlink(MSG_Q_Name_GYM);
    if ((msgq_fd  = mq_open (MSG_Q_Name_GYM, O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        printf ("Client: mq_open failed, errno = %d", errno);
        exit (1);
    }
    spdlog::info("Client::"+str(__FUNCTION__)+"::end");
    
}
void Server_MQ::run(){
 
    while(1){
        spdlog::info("Server::"+str(__FUNCTION__));
        FD_ZERO(&readfds);
        FD_SET( msgq_fd, &readfds);
        cout<<"Reciever blocked on select()....\r";
        cout<<"msgq_fd + 1::"<<msgq_fd + 1<<endl;
        select(msgq_fd + 1, &readfds, NULL, NULL, NULL);
        cout<<"msgq_fd + 1::"<<msgq_fd + 1<<endl;
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
            spdlog::info("Client::"+str(__FUNCTION__)+"::end");
        }
    }
    
}

bool Server_MQ::create_run_thread(){
      spdlog::info("Server::"+str(__FUNCTION__));
    bool success = pthread_create(&t,NULL,InternalThreadEntryFunc,this)==0;
     spdlog::info("Client::"+str(__FUNCTION__)+"::end");
    return success;
}

void * Server_MQ::InternalThreadEntryFunc(void * This){
      spdlog::info("Server::"+str(__FUNCTION__));
     ((Server_MQ *)This)->run(); 
      spdlog::info("Client::"+str(__FUNCTION__)+"::end");
     return NULL;

 }


bool Server_MQ::is_there_any_client_in_queue(){
      spdlog::info("Server::"+str(__FUNCTION__));
    pthread_mutex_lock(&this->clients_queue.lock);
    if (clients_queue.data.size() ==0){
        pthread_mutex_unlock(&this->clients_queue.lock);
         spdlog::info("Client::"+str(__FUNCTION__)+"::end");
        return false;
    }else{
        pthread_mutex_unlock(&this->clients_queue.lock);
         spdlog::info("Client::"+str(__FUNCTION__)+"::end");
        return true;
    }
}
bool Server_MQ::is_client_in_queue(boost::uuids::uuid client_id,bool keep_mutex_locked){
      spdlog::info("Server::"+str(__FUNCTION__));
    cout<<"I am here::is_client_in_queue"<<endl; 
    // pthread_mutex_lock(&this->clients_queue.lock);
    if (clients_queue.data.size() ==0){
        cout<<"I am here::is_client_in_queue::false::1"<<endl; 
        if (!keep_mutex_locked) pthread_mutex_unlock(&this->clients_queue.lock);
        spdlog::info("Client::"+str(__FUNCTION__)+"::end");
       
        return false;
    }else{
        for(int i = 0; i < clients_queue.data.size(); ++i){
            string str1 = boost::lexical_cast<std::string>(clients_queue.data[i]);
            string str2=boost::lexical_cast<std::string>(client_id);
            bool same = !strcmp(str1.c_str(), str2.c_str());
            cout<<"clients_queue.data[i]::"<<boost::lexical_cast<std::string>(clients_queue.data[i])<<endl;
            cout<<"client_id::"<<boost::lexical_cast<std::string>(client_id)<<endl;
            cout<<"same::"<< std::boolalpha << same <<endl;
            if (same){

                cout<<"I am here::is_client_in_queue::True"<<endl;
                if (!keep_mutex_locked) pthread_mutex_unlock(&this->clients_queue.lock);
                spdlog::info("Client::"+str(__FUNCTION__)+"::end");
                return true;
            }
                
        }
        cout<<"I am here::is_client_in_queue::false::2"<<endl;
        if (!keep_mutex_locked) pthread_mutex_unlock(&this->clients_queue.lock);
        spdlog::info("Client::"+str(__FUNCTION__)+"::end");
        return false;
   
    }
}

bool Server_MQ::is_gymState_in_state_buffer(boost::uuids::uuid client_id){
    spdlog::info("Server::"+str(__FUNCTION__));
    pthread_mutex_lock(&state_buffer_mutex);
    if  (state_buffer.size()==0){
        pthread_mutex_unlock(&state_buffer_mutex);
         spdlog::info("Client::"+str(__FUNCTION__)+"::end");
        return false;
    }else{
        for(int i = 0; i < state_buffer.size(); ++i){
            if (state_buffer[i].key.id==client_id){
                 pthread_mutex_unlock(&state_buffer_mutex);
                 spdlog::info("Client::"+str(__FUNCTION__)+"::end");
                    return true;

            }
        }
        pthread_mutex_unlock(&state_buffer_mutex);
         spdlog::info("Client::"+str(__FUNCTION__)+"::end");
        return false;
    }
        
    
}
void Server_MQ::add_client_to_queue(boost::uuids::uuid client_id){
      spdlog::info("Server::"+str(__FUNCTION__));
    //add client to queue if it is not already in queue 
    pthread_mutex_lock(&this->clients_queue.lock);
    cout<<"I am here::add_client_to_queue"<<endl; 
    bool flag=!is_client_in_queue(client_id,true);
    if (flag){
        // pthread_mutex_lock(&this->clients_queue.lock);
        // cout<<"add_client_to_queue::locking the resource"<<endl;
        clients_queue.data.push_back(client_id);
       
        pthread_mutex_unlock(&this->clients_queue.lock);
        cout<<"add_client_to_queue::unlocking the resource"<<endl;
    }
    pthread_mutex_unlock(&this->clients_queue.lock);
     spdlog::info("Client::"+str(__FUNCTION__)+"::end");
   
}

void Server_MQ::add_client_state_to_buffer(boost::uuids::uuid client_id,GymworldState gymState){
    //update state if already in buffer or add state if not  
      spdlog::info("Server::"+str(__FUNCTION__));
    
    // cout<<"i am here::add_client_state_to_buffer"<<endl;
    if (is_gymState_in_state_buffer(client_id)){
        pthread_mutex_lock(&state_buffer_mutex);
        for(int i = 0; i < state_buffer.size(); ++i){
            if (state_buffer[i].key.id==client_id){
            //update state
            state_buffer[i] =gymState;
            }
        }
    }else{
         state_buffer.push_back(gymState);
    }
    pthread_mutex_unlock(&state_buffer_mutex);
     spdlog::info("Client::"+str(__FUNCTION__)+"::end");
    

}
void Server_MQ::remove_client_from_queue(boost::uuids::uuid client_id){
      spdlog::info("Server::"+str(__FUNCTION__));
   
    pthread_mutex_lock(&this->clients_queue.lock);
    for(int i = 0; i < clients_queue.data.size(); ++i){
        if (clients_queue.data[i]==client_id){
            
            clients_queue.data.erase(clients_queue.data.begin()+i);
            
            
        }
    }
    pthread_mutex_unlock(&this->clients_queue.lock);
     spdlog::info("Client::"+str(__FUNCTION__)+"::end");
    

}
void Server_MQ::removeStateFromBuffer(boost::uuids::uuid client_id){
         spdlog::info("Server::"+str(__FUNCTION__));     
    for(int i = 0; i < state_buffer.size(); ++i){
        if (state_buffer[i].key.id==client_id){
            //remove state
            pthread_mutex_lock(&state_buffer_mutex);
            state_buffer.erase(state_buffer.begin()+i);
            pthread_mutex_unlock(&state_buffer_mutex);
        }

    }
     spdlog::info("Client::"+str(__FUNCTION__)+"::end");

}
vector <boost::uuids::uuid> Server_MQ::getClientsInQueue(){
  spdlog::info("Server::"+str(__FUNCTION__));
   cout<<"server::calling::getClientsInQueue"<<endl;
   vector <boost::uuids::uuid> clients_queue_ids;
    pthread_mutex_lock(&this->clients_queue.lock);
    cout<<"getClientsInQueue::locking the resource"<<endl;
    for(int i = 0; i < clients_queue.data.size(); ++i){
        
     
        clients_queue_ids.push_back(clients_queue.data[i]);

    }
    cout<<"getClientsInQueue::unlocking the resource"<<endl;
    pthread_mutex_unlock(&this->clients_queue.lock);
     spdlog::info("Client::"+str(__FUNCTION__)+"::end");
    return clients_queue_ids;
}

GymworldState Server_MQ::getGymStateforId(boost::uuids::uuid client_id){
      spdlog::info("Server::"+str(__FUNCTION__));
    pthread_mutex_lock(&state_buffer_mutex);
    // cout<<"i am here::getGymStateforId"<<endl;
    for(int i = 0; i < this->state_buffer.size(); ++i){
        // cout<<"i am here::getGymStateforId::I am insdie loop"<<endl;
        if (this->state_buffer[i].key.id==client_id){
            //remove state 
            GymworldState state = this->state_buffer[i];
            pthread_mutex_unlock(&state_buffer_mutex);
             spdlog::info("Client::"+str(__FUNCTION__)+"::end");
            return state;
        }

    }
    pthread_mutex_unlock(&state_buffer_mutex);
     spdlog::info("Client::"+str(__FUNCTION__)+"::end");
   
}

bool Server_MQ::send(boost::uuids::uuid client_id,GhostWorldState ghostState){
    spdlog::info("Server::"+str(__FUNCTION__));
    string client_mqName = MSG_Q_Name_GHOST+boost::uuids::to_string(client_id);
    ghostState.key.id = client_id;
    ghostState.key.mqName = client_mqName;

    cout<<"server::send::q_name:: "<<client_mqName<<endl;

    milliseconds ms = duration_cast< milliseconds >(
    system_clock::now().time_since_epoch());
    ghostState.key.timestep =ms;


    //preparing the data
    char data[BUFFER_SIZE];
    strcpy(data,searlalizer->ghostState(ghostState).c_str());

    //Creating a message queue
    if ((recvr_msgq_fd  = mq_open (client_mqName.c_str(), O_WRONLY | O_CREAT, 0, 0)) == -1) {
        printf ("Client: mq_open failed, errno = %d", errno);
        exit (1);
    }
    
    //Adding message to the Queue
    if (mq_send (recvr_msgq_fd, data, strlen (data) + 1, 0) == -1) {
        perror ("Client: Not able to send message to server");
        exit (1);
    }
    mq_close(recvr_msgq_fd);
     spdlog::info("Client::"+str(__FUNCTION__)+"::end");
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

    spdlog::info("Client::"+str(__FUNCTION__));
    //Creating a Message Queue In order to listen to messagees coming form server

      //Creating a Message Queue In order to listen to messagees coming form server
    this->mqName = generate_mqName_for_this_client(this->id);

    /*To set msgQ attributes*/
    attr.mq_flags = 0;
    attr.mq_maxmsg = MAX_MESSAGES;
    attr.mq_msgsize = MAX_MSG_SIZE;
    attr.mq_curmsgs = 0;
     spdlog::info("Client::"+str(__FUNCTION__)+"::end");

}
void Client_MQ::init(){
//TODO
  spdlog::info("Client::"+str(__FUNCTION__));
    cout<<"client::q_name:: "<<this->mqName.c_str()<<endl;
    if ((msgq_fd  = mq_open (this->mqName.c_str(), O_RDONLY | O_CREAT, QUEUE_PERMISSIONS, &attr)) == -1) {
        printf ("Client: mq_open failed, errno = %d", errno);
        exit (1);
    }
     spdlog::info("Client::"+str(__FUNCTION__)+"::end");
    
}
void Client_MQ::run(){
//TODO
  
    while(1){
        spdlog::info("Client::"+str(__FUNCTION__));
        FD_ZERO(&readfds);
        FD_SET( msgq_fd, &readfds);
        printf("Reciever blocked on select()....\n");
        cout<<"msgq_fd + 1::"<<msgq_fd + 1<<endl;
        select(msgq_fd + 1, &readfds, NULL, NULL, NULL);
        if(FD_ISSET(msgq_fd, &readfds)){
            \
            
            memset(buffer, 0, MSG_BUFFER_SIZE);
            if (mq_receive (msgq_fd, buffer, MSG_BUFFER_SIZE, NULL) == -1) {
                printf ("mq_receive error, errno = %d\n", errno);
                exit (1);
            }
            printf("Msg recieved from Queue = %s\n", buffer);
            /* Recive Ghost State*/
            GhostWorldState ghost_State;
            
            char data_c[BUFFER_SIZE];
            // cout<<"buffer::"<<buffer<<endl;
            memcpy(&data_c, buffer,BUFFER_SIZE);
            //parse data
            ghost_State = parser->parseGhostState(data_c);
            cout<<"ghost_State::ff::"<<ghost_State.ff<<endl;

            //update Ghost state
            updateState(ghost_State);
        }
     spdlog::info("Client::"+str(__FUNCTION__)+"::end");
    }
    
}
bool Client_MQ::send(GymworldState gymState){
    spdlog::info("Client::"+str(__FUNCTION__));
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
    spdlog::info("Client::"+str(__FUNCTION__)+"::end");
    return true;
}
bool Client_MQ::create_run_thread(){
      spdlog::info("Client::"+str(__FUNCTION__));
    bool success = pthread_create(&t,NULL,InternalThreadEntryFunc,this)==0;
  spdlog::info("Client::"+str(__FUNCTION__)+"::end");
    return success;
}

void * Client_MQ::InternalThreadEntryFunc(void * This){
      spdlog::info("Client::"+str(__FUNCTION__));
     ((Client_MQ *)This)->run(); 
       spdlog::info("Client::"+str(__FUNCTION__)+"::end");
     return NULL;

 }
string Client_MQ::generate_mqName_for_this_client(boost::uuids::uuid id){
    //To make sure message queue name is unique we use cliend id as part of the name
     spdlog::info("Client::"+str(__FUNCTION__));
   return  MSG_Q_Name_GHOST+ boost::uuids::to_string(id);
}
boost::uuids::uuid Client_MQ::generate_UUID_for_this_client(){
      spdlog::info("Client::"+str(__FUNCTION__));
     return   boost::uuids::random_generator()();
}
void Client_MQ::updateState(GhostWorldState ghost_State){
      spdlog::info("Client::"+str(__FUNCTION__));
    pthread_mutex_lock(&state_mutex);
    this->ghostState = ghost_State;
    cout<<"Client_MQ::updateState::ghost_State::id"<<boost::uuids::to_string(ghost_State.key.id)<<endl;
    bool id_is_a_match =boost::uuids::to_string(ghost_State.key.id) == boost::uuids::to_string(this->id);
    cout<<"id_is_a_match:: "<<std::boolalpha <<  id_is_a_match<<endl;
    pthread_mutex_unlock(&state_mutex);
      spdlog::info("Client::"+str(__FUNCTION__)+"::end");
}
GhostWorldState Client_MQ::getGhostStateforClient(){
     //TODO
       spdlog::info("Client::"+str(__FUNCTION__));
    //deque new state form message queue
    pthread_mutex_lock(&state_mutex);
    GhostWorldState ghost_state = this->ghostState;
    pthread_mutex_unlock(&state_mutex);
  spdlog::info("Client::"+str(__FUNCTION__)+"::end");
    return ghost_state;
    
 }
