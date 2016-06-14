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



#define DBG
#ifdef DBG
#define debug_print(...) fprintf(stderr, __VA_ARGS__);
#else
#define debug_print(...)
#endif
#define DEFAULT_PORT 1337

#endif // _COMMON_H
