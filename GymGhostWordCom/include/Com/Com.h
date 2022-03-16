#include "msg.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <iostream>
#include <string> 
#include <pthread.h>


#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <errno.h>


#include<pthread.h>


#include <jsoncpp/json/json.h>


#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 1000

#define QUEUE_PERMISSIONS   0660
#define MAX_MESSAGES 10

//********This is like ros publish topic***********
#define MSG_Q_Name_GYM "/msq_gym"
#define MSG_Q_Name_GHOST "/msq_ghost"
//************************************************
#define MAX_MSG_SIZE        1000
#define MSG_BUFFER_SIZE     (MAX_MSG_SIZE + 10)

#define MAX_CLIENT_SUPPORTED    32

using namespace std;

struct Cleint_queue{
       vector <boost::uuids::uuid> data;
        pthread_mutex_t lock;
};

class Parser{
    public:
        GymworldState parseGymState(char data[]);
        GhostWorldState parseGhostState(char data[]);

};


class Searlalize{
    GymworldState gym_State;
    int buffer_length = sizeof(gym_State.ff.pos.x)+sizeof(gym_State.ff.pos.y)+sizeof(gym_State.ff.pos.z)+sizeof(gym_State.ff.orn.r)+sizeof(gym_State.ff.orn.p)+sizeof(gym_State.ff.orn.y);
    public:
        string gymState(GymworldState gym_State);
        string ghostState(GhostWorldState ghost_State);
        string uuid_s(boost::uuids::uuid client_id);
        int getGymStateBufferSize();
};

//****************Unix Domain Sockets**************************
class Server{
    private:

        int monitored_fd_set[MAX_CLIENT_SUPPORTED];
        GymworldState client_state[MAX_CLIENT_SUPPORTED] ;
        //id and socket pair
       
        
        sockaddr_un name;
        Parser* parser =new Parser();
        int ret;
        int connection_socket;
        int data_socket;
        int result;
        string data;
        string world;
        char buffer[BUFFER_SIZE];
        fd_set readfds;
        int comm_socket_fd, i;
        Searlalize* searlalizer = new Searlalize();
        bool terminate_server_flag= false;

        pthread_t t;

        static void * InternalThreadEntryFunc(void * This);


  

    public:
        bool serverIsReady = false;
        vector <GymworldState> state_buffer;
        vector <pair<boost::uuids::uuid,int> > clients_queue;
        bool create_run_thread();
        void init();
        void run();
       
        void intitiaze_monitor_fd_set();
        void add_to_monitored_fd_set(int skt_fd);
        void remove_from_monitored_fd_set(int skt_fd);
        void refresh_fd_set(fd_set *fd_set_ptr);
        int get_max_fd();

        void GhostHandle();
        void terminate_server();
        bool send(boost::uuids::uuid client_id,GhostWorldState ghostState); 
        bool is_there_any_client_in_queue();
        bool is_client_in_queue(boost::uuids::uuid client_id);
        bool is_client_in_queue(int client_socket);
        bool is_gymState_in_state_buffer(boost::uuids::uuid client_id);
        void add_client_to_queue(boost::uuids::uuid client_id,int socket);
        void add_client_state_to_buffer(boost::uuids::uuid client_id,GymworldState gymState);
        void remove_client_from_queue(boost::uuids::uuid client_id);
        void removeStateFromBuffer(boost::uuids::uuid client_id);
        vector <boost::uuids::uuid> getClientsInQueue();
        int get_socket_for_client(boost::uuids::uuid client_id);
        GymworldState getGymStateforId(boost::uuids::uuid client_id);
        Server();

};

class Client{
    private:
        struct sockaddr_un addr;
        int i;
        int ret;
        int data_socket;
        char buffer[BUFFER_SIZE];
        boost::uuids::uuid id;

        Searlalize* searlalizer = new Searlalize();
        

    public:
        boost::uuids::uuid generate_UUID_for_this_client();
        bool send(GymworldState gymState);

    Client();

};

//****************Masage Queue**************************
class Server_MQ{
    private:
        fd_set readfds;
        char buffer[MSG_BUFFER_SIZE];
        int msgq_fd = 0;
        int recvr_msgq_fd = 0;
        struct mq_attr attr;
        Parser* parser =new Parser();
        Searlalize* searlalizer = new Searlalize();

        pthread_t t;
        pthread_mutex_t state_buffer_mutex;
        pthread_mutex_t clients_queue_mutex;

        static void * InternalThreadEntryFunc(void * This);




    public:

        Cleint_queue clients_queue;
        
        vector <GymworldState> state_buffer;
        bool create_run_thread();
        void init();
        void run();

        bool is_there_any_client_in_queue();
        bool is_client_in_queue(boost::uuids::uuid client_id,bool keep_mutex_locked);
        bool is_gymState_in_state_buffer(boost::uuids::uuid client_id);
        void add_client_to_queue(boost::uuids::uuid client_id);
        void add_client_state_to_buffer(boost::uuids::uuid client_id,GymworldState gymState);
        void remove_client_from_queue(boost::uuids::uuid client_id);
        void removeStateFromBuffer(boost::uuids::uuid client_id);
        vector <boost::uuids::uuid> getClientsInQueue();
        GymworldState getGymStateforId(boost::uuids::uuid client_id);
        bool send(boost::uuids::uuid client_id,GhostWorldState ghostState); 
        Server_MQ();

};
class Client_MQ{
    private:
        fd_set readfds;
        char buffer[MSG_BUFFER_SIZE];
        int msgq_fd = 0;
        int recvr_msgq_fd = 0;
        GhostWorldState ghostState;

        struct mq_attr attr;
        Parser* parser =new Parser();

        pthread_t t;
        pthread_mutex_t state_mutex;

        static void * InternalThreadEntryFunc(void * This);

        boost::uuids::uuid id; //client id
        string mqName; //client message queue name
        Searlalize* searlalizer = new Searlalize();
    public:
        bool create_run_thread();
        void init();
        void run();
        string generate_mqName_for_this_client(boost::uuids::uuid id);
        boost::uuids::uuid generate_UUID_for_this_client();
        string generate_UUID_for_this_client(boost::uuids::uuid id);
        bool send(GymworldState gymState);
        GhostWorldState getGhostStateforClient();
        void updateState(GhostWorldState ghost_State);

        Client_MQ();
};