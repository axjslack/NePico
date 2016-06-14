/*
 * Server TCP IP for pincontrol
 * The server is no-multithread. This is not an error is necessary.
 * I'm unable to garantee the concurrency on pin manage
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr

#include "common.h"
#include "pinmesg.h"
#include "gpiocontrol.h"
#include "confparser.h"

void null_function()
{
	//Here only for debug purpose
}


int server_main(selector *sel)
{
    int socket_desc,new_socket, c, dfl_msgsize;
    struct sockaddr_in server, client;
    pinmsg *to_rec;
    
    dfl_msgsize=sizeof(pinmsg);
    to_rec=malloc(dfl_msgsize);
    
     
    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        error_print("Could not create socket");
    }
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( DEFAULT_PORT );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        error_print("bind failed");
        return 1;
    }
    error_print("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    error_print("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
    while( (new_socket = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        error_print("Connection accepted");
         
        //Reply to the client
        //message = "Hello Client , I have received your connection. But I have to go now, bye\n";
        //write(new_socket , message , strlen(message));
		if(recv(new_socket, (void *)to_rec , dfl_msgsize , MSG_WAITALL) < 0)
		{
			error_print("recv failed");
		}
		print_message(to_rec);
		switch(to_rec->slt) {
			case NULL_CMD : null_function(); break;
			case SC_CMD : decode_mesg(sel, to_rec); break;
			default : null_function();
		}	
    }
     if (new_socket<0)
    {
        perror("accept failed");
        return 1;
    }
     
    return 0;
}


int main(int argc, char *argv[])
{
#ifdef LOCALMAPPING
	selector *localpin;
	
	error_print("\n NePiCo alpha (test) \n");


	localpin=malloc(sizeof(selector));
	init_localpin(localpin);
	setdirection_localpin(localpin, OUT);
	//multiled_kr_7p(localpin);
	server_main(localpin);
#else
	selector *confpin;
	int pos;
	
	char *serport=NULL, *baud=NULL, *ports=NULL; //Serial port configuration parameter
	
	error_print("\n NePiCo beta (test) \n");
	//Configuration of the GPIOs as pin
	
	confpin=malloc(sizeof(selector));
	pos=running_conf(confpin);
	init_confpin(confpin, pos);
	setdirection_confpin(confpin, pos);
	
	
	//Enabling the server
    
    //Serial part
    //TODO
    
    
    
    
    //Network Part
	server_main(confpin);
	



#endif	
	return 0; 
}


