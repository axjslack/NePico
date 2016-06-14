#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "rs232.h"


int select_serial(char *serport)
{
	int ttyn;

	if(strcmp(serport,"ttyS0")== 0) {ttyn= 0; }
	else if(strcmp(serport, "ttyS1")== 0) {ttyn= 1; }
	else if(strcmp(serport, "ttyS2")== 0) {ttyn= 2; }
	else if(strcmp(serport, "ttyS3")== 0) {ttyn= 3; }
	else if(strcmp(serport, "ttyS4")== 0) {ttyn= 4; }
	else if(strcmp(serport, "ttyUSB0")== 0) {ttyn= 16; }
	else if(strcmp(serport, "ttyUSB1")== 0) {ttyn= 17; }
	else if(strcmp(serport, "ttyUSB2")== 0) {ttyn= 18; }
	else if(strcmp(serport, "ttyUSB3")== 0) {ttyn= 19; }
	else if(strcmp(serport, "ttyAMA0")== 0) {ttyn= 22; }
	else if(strcmp(serport, "ttyAMA1")== 0) {ttyn= 23; }
	else if(strcmp(serport, "ttyACM0")== 0) {ttyn= 24; }
	else if(strcmp(serport, "ttyACM1")== 0) {ttyn= 25; }
	else
		error_print("\nSerial port not supported\n");

	return ttyn;

}



int serial_connect(serial_port_t sport, uint8_t *serial_pinmsg)
{

	int result,count;
#ifdef DBG
	int i;
#endif	


	if(RS232_OpenComport(sport.ttyn, sport.bdrate, sport.mode))
	  {
	    error_print("Can not open comport\n");
	    result=404;
	  }

	sleep(1);

	count=RS232_PollComport(sport.ttyn, reply, sizeof(pinmsg));
	

#ifdef DBG
	error_print("\n%d byte read from %d at %d Kb/s \n", count, sport.ttyn, sport.bdrate);
	for(i=0;i<count;i++)
	{
		error_print(" %X ", reply[i]);
	}
#endif
	return result;

}
