/*#include <stdio.h>
#include "common.h"
#include "gpiocontrol.h"
#include <stdlib.h>


int main(int argc, char *argv[])
{
	selector *localpin;
	
	error_print("\n NePiCo alpha (test) \n");
	
	localpin=malloc(sizeof(selector));
	init_localpin(localpin);
	setdirection_localpin(localpin, OUT);
	//multiled_kr_7p(localpin);
	server_main(localpin);
	
	return 0; 
}


*/
