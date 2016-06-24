#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>

#define error_print(...) fprintf(stderr, __VA_ARGS__);
//#define debug_print(...) fprintf(stderr, __VA_ARGS__);


typedef struct serial_port {
	int ttyn;
	int bdrate;
	char mode[4];
} serial_port_t;

typedef enum eConn_type {
	serial,
	network,
	undef
} Conn_type_t; 

typedef enum eIP_type {
	ipv4,
	ipv6,
	unspec
} IP_type_t;


int select_serial(char *serport);

#define DBG
#ifdef DBG
#define debug_print(...) fprintf(stderr, __VA_ARGS__);
#else
#define debug_print(...)
#endif
#define DEFAULT_PORT 1337

#endif // _COMMON_H
