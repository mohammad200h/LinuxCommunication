#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
//  #include "msg.h"
#include "Com/Com.h"
#include <iostream>

#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 400

using namespace std;






int len(char a[]){

    return sizeof(a)/sizeof(a[0]);
}


int
main(int argc, char *argv[])
{
    GymworldState gymState;
    gymState.ff.pos.x = 10;
    int min = -5;
    int max = 5;

    cout<<"Runing Client.."<<endl;
    Client* client = new Client();  
    
    while(1){
        gymState.ff.pos.z = 5;
        bool succ = client->send(gymState);
    }

    

         
    
   
    cout<<"I am done client"<<endl;
    return 0;

}
