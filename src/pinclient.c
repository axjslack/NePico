#include <sys/socket.h>
#include <arpa/inet.h>	//inet_addr
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdint.h>

#include "pinmesg.h"
#include "common.h"




int send_pinmesg(pinmsg to_send, int port,  char* ip_add)
{
	int read_size, socket_desc;
	struct sockaddr_in server;


	printf("DEBUG: Dest IP %s port %d\n", ip_add, port);
	
		//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		error_print("\nCould not create socket\n");
		return 1;
	}
		
	//server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_addr.s_addr = inet_addr(ip_add);
	server.sin_family = AF_INET;
	server.sin_port = htons( port );
	
	//Connect to remote server
	if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		error_print("\nconnection error\n");
		return 2;
	}
	error_print("\nConnected\n");
	
	if( send(socket_desc , (void*)&to_send , sizeof(pinmsg), 0) < 0)
	{
		error_print("\nSend failed");
		return 1;
	}
	error_print("\nData Send");
	
	close(socket_desc);
	
	return 0;
}



int main(int argc, char *argv[])
{
	uint32_t setv, clearv;
	int c, port;
	char *ip=NULL;
	pinmsg to_send;
	
	while ((c = getopt (argc, argv, "p:i:s:c:")) != -1)
	{
		switch (c)
        {
			
			
			case 'p': port = atoi(optarg); break;
			case 'i': ip = optarg; break;
			case 's': to_send.sbit=atoi(optarg); break;
			case 'c': to_send.cbit=atoi(optarg); break;
			//case 'f': filename = optarg; break;
			default:
				//print_usage();
				return 1;
        }
	}
	error_print("\n sbit: %lu cbit: %lu \n", to_send.sbit, to_send.cbit);
	
	to_send.slt=1;
	
	send_pinmesg(to_send, port, ip); 
	
	return 0;
}

