/*
 * Server TCP IP for pincontrol
 * The server is no-multithread. This is not an error: is necessary.
 * I'm unable to garantee the concurrency on pin management
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

void print_usage()
{
	error_print("\n nepico_server parameters \n");
	error_print("\n Network Connection parameter\n");
	error_print("-n :\t\t set network connection\n");
	error_print("-i <ipv4|ipv6> :\t\t set the ip protocol\n");
	error_print("-p <port> :\t\t set the liestening port \n");
	error_print("\n Serial Connection parameter\n");
	error_print("-s :\t\t set serial connection\n");
	error_print("-c <ttyport> :\t\t set the serial port\n");
	error_print("-b <port> :\t\t set the connection speed (baud)\n");

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

void serial_main(serial_port_t sport, selector *confpin)
{
		
}





int main(int argc, char *argv[])
{
	
/* The local mapping will disappear in the stable release */	
#ifdef LOCALMAPPING
	selector *localpin;
	
	error_print("\n NePiCo alpha (test) \n");


	localpin=malloc(sizeof(selector));
	init_localpin(localpin);
	setdirection_localpin(localpin, OUT);
	server_main(localpin);
#else
	selector *confpin;
	int pos;
	
	/*Serial port or network configuration parameter*/
	char c,*serport=NULL, *baud=NULL, *ports=NULL, *ip="ipv4"; 
	Conn_type_t	connection=undef;
	serial_port_t	sport;
	IP_type_t	iptype;
	
	error_print("\n NePiCo beta (test) \n");
	
	
	
	//Configuration of the GPIOs as pin
	
	confpin=malloc(sizeof(selector));
	pos=running_conf(confpin);
	init_confpin(confpin, pos);
	setdirection_confpin(confpin, pos);
	
	
	
	while ((c = getopt (argc, argv, "snc:b:p:i:")) != -1)
	{
		switch(c)
		{
			case 's': connection=serial; break;
			case 'n': connection=network; break;
			case 'c': serport = optarg; break;
			case 'b': baud = optarg; break;
			case 'p': ports = optarg; break;
			case 'i': ip = optarg; break;
			
		}
		
		
	}
	
	
	if(connection==undef)
	{
		error_print("\n No connection type selected \n");
		print_usage();
		return 2;
	}


	if(connection==serial)
	{

		if(serport==NULL || baud==NULL )
		{
			error_print("\n Serial connection need of a serial port  and a baud rate\n");
			print_usage();
			return 4;
		}
		else
		{
			sport.ttyn=select_serial(serport);
			sport.bdrate=atoi(baud);
			sport.mode[0]='8';
			sport.mode[1]='N';
			sport.mode[2]='1';
			sport.mode[3]=0;
			serial_main(sport, confpin);
		}
	}

	if(connection==network)
	{
		if(ports==NULL || ip==NULL )
		{
			error_print("\n For a correct network connection, an IP and a port are needed\n");
			print_usage();
			return 4;
		}
		else
		{
			server_main(confpin);
		}
	}
#endif	
	return 0; 
}


