#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>



#include "gpiocontrol.h"
#include "pinmesg.h"
#ifdef LOCALMAPPING
#include "localmap.h"
#endif 
#include "common.h"


//Library function

int reserve_gpio(gpn n)
{
	int fd;
	char buf[PATH_BUF]; 

	fd = open("/sys/class/gpio/export", O_WRONLY);
	if ( fd < 1 )
	{
		error_print("Unable to open GPIO export file: %s",buf);
		return 1;
	}
	sprintf(buf, "%d", n); 
	write(fd, buf, strlen(buf));
    close(fd);
	return 0;
}

int set_direction(gpn n, gpd d)
{
	int fd;
	char buf[PATH_BUF];
	
	sprintf(buf, "/sys/class/gpio/gpio%d/direction", n);
	fd = open(buf, O_WRONLY);
	if ( fd < 1 )
	{
		error_print("Unable to open GPIO direction file: %s", buf);
		return 1;
	}

	if( d == OUT ){write(fd, "out", 3); }
	if( d == IN ){write(fd, "in", 2); }
	close(fd);		

return 0; 
}


gpd check_direction(gpn n)
{
	int fd;
	char buf[PATH_BUF];
	char value[2];
	
	sprintf(buf, "/sys/class/gpio/gpio%d/direction", n);
	fd = open(buf, O_RDONLY);
	if ( fd < 1 )
	{
		error_print("Unable to open GPIO direction file: %s", buf);
		return ERR;
	}
	
	read(fd, value, 2);
	
	if(strcpy(value, "ou"))
	{
		return OUT;
	}	
	else
	{
		return IN;
	}	
return ERR;
	
}



int write_gpio(gpn n, gpv v)
{
	char value;
	char buf[PATH_BUF];
	int fd;

	sprintf(buf, "/sys/class/gpio/gpio%d/value", n);
	fd = open(buf, O_WRONLY);
	sprintf(&value, "%d", v);
	write(fd, &value, strlen(&value));
    close(fd);
return 0;
}

gpv read_gpio(gpn n)
{
	char value;
	char buf[PATH_BUF];
	int fd;
	gpv retgpio;
	
	retgpio=OFF;

	sprintf(buf, "/sys/class/gpio/gpio%d/value", n);
	fd = open(buf, O_RDONLY);
	read(fd, &value, 1);

	if(value == '0'){retgpio=OFF;}	
	else
		{ retgpio=ON;}
	//printf("Value 1\n");	
	close(fd);
	return retgpio;
}


int poll_gpio(gpn n)
{
	char value;
	char buf[PATH_BUF];
	int fd;
	gpv v;
	
	//Init GPIO
	
	printf("DEBUG: Reserving GPIO \n");
	reserve_gpio(n);
	printf("DEBUG: Reserved GPIO \n");
	set_direction(n, IN);
	printf("DEBUG: Direction set GPIO \n");
	
	
	//Open GPIO
	sprintf(buf, "/sys/class/gpio/gpio%d/value", n);
	fd = open(buf, O_RDONLY);
	if ( fd < 1 )
	{
		error_print("Unable to open GPIO vale file: %s", buf);
		return 1;
	}
	
	while( v != ON )
	{
		read(fd, &value, 1);
		printf("DEBUG: Value of gpio is char  %c int %d\n",value, value);
		if(!strcmp(&value,"1"))
			{ v = ON; }	
		else
			{ v = OFF; }
		usleep(500000);
	}
	usleep(500000);
	close(fd);
return 0;	
}


int led_pulse(gpn n)
{
	int i;
	
	for(i=0;i<LED_TIME;i++)
	{
		//Assuming GPIO is enabled, reserved, and in OUT Direction 
		write_gpio(n, ON);
		usleep(500000);
		write_gpio(n, OFF);
		usleep(500000);
	}
	return 0;
}

/*
 * This code is coming from MoM and need to be cleaned 
 * 
  gpn get_gpiomap(int i);
{
	return 44+i;
}


void direct_pin_init(dpin_array *pins)
{
	int i;
	
	for(i=0; i<MAX_DIRECT_PIN;i++)
	{
		pins->pin_array[i].gpion=get_gpiomap(i);
		pins->pin_array[i].gpiodir=OUT;
		pins->pin_array[i].gpiov=OFF;
	}		
	
}
*/



//Local functions

#ifdef LOCALMAPPING

uint8_t init_localpin(selector *localpin)
{	
	int i;
	
	localpin->pin_n[0].gpion=GPIO1;
	localpin->pin_n[1].gpion=GPIO2;
	localpin->pin_n[2].gpion=GPIO3;
	localpin->pin_n[3].gpion=GPIO4;
	localpin->pin_n[4].gpion=GPIO5;
	localpin->pin_n[5].gpion=GPIO6;
	localpin->pin_n[6].gpion=GPIO7;
	localpin->pin_n[7].gpion=MAX_PIN;


	//set output
	for(i=0;i<7;i++)
	{
		error_print("\n Asking for GPIO %d Reservation", localpin->pin_n[i].gpion);
		reserve_gpio(localpin->pin_n[i].gpion);
	}	
	
	return 0;
}

void setdirection_localpin(selector *localpin, gpd direction)
{
	int i; 
	
	for(i=0;i<6;i++)
	{
		error_print("\n Asking for GPIO %d Direction", localpin->pin_n[i].gpion);
		set_direction(localpin->pin_n[i].gpion, direction);
	}	
}


void deinit_localpin(selector *localpin)
{
	
}

#endif //LOCALMAPPING

void init_confpin(selector *confpin, int pos)
{
	int i;
	
	for(i=0; i<pos; i++)
	{
		reserve_gpio(confpin->pin_n[i].gpion);
	}
	
}

void setdirection_confpin(selector *confpin, int pos)
{
	int i; 
	
	for(i=0;i<pos;i++)
	{
		error_print("\n Asking for GPIO %d Direction", confpin->pin_n[i].gpion);
		set_direction(confpin->pin_n[i].gpion, confpin->pin_n[i].direction);
	}	
}



uint8_t multiled_kr_7p(selector *localpin)
{	
	int i=0, nleds;
	gpv v;
	
	nleds=6;
	
	//setdirection for pins
	for(i=0;i<nleds;i++)
	{
		error_print("\n Asking for GPIO %d Direction", localpin->pin_n[i].gpion);
		set_direction(localpin->pin_n[i].gpion, OUT);
	}
	error_print("\n Asking for GPIO %d Direction", localpin->pin_n[nleds].gpion);	
	set_direction(localpin->pin_n[nleds].gpion, IN);

	while(1) //Main loop
	{
		v=read_gpio(localpin->pin_n[nleds].gpion);
		if( v == ON )
		{
			printf("GPIO %d  value is %d state\n", localpin->pin_n[nleds].gpion, ((int)v));
			usleep(500000);
			write_gpio(localpin->pin_n[0].gpion, ON);
			for(i=0;i<nleds-1;i++)
			{
				usleep(500000);
				write_gpio(localpin->pin_n[i+1].gpion, ON);
				usleep(100000);
				write_gpio(localpin->pin_n[i].gpion, OFF);
			}	
			for(i=nleds-2;i>=0;i--)
			{
				usleep(500000);
				write_gpio(localpin->pin_n[i].gpion, ON);
				usleep(100000);
				write_gpio(localpin->pin_n[i+1].gpion, OFF);
			}
			write_gpio(localpin->pin_n[0].gpion, OFF);

		}
	}

return 0;	
	
}	
		
		


//Remote functions



void set_pin_cycle(selector *localpin, uint8_t *oplist_static, gpv op)
{
	int i=0;
	
	while(oplist_static[i] != END_OF_LIST)
	{	
		debug_print("Oplist position %d: GPIO number %d, operation %d",i,localpin->pin_n[oplist_static[i]].gpion, op);
		write_gpio(localpin->pin_n[oplist_static[i]].gpion, op);
		i++;
	}		
}

