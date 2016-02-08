
#include "gpiocontrol.h"
#include "pinmesg.h"
#include "common.h"

//uint8_t oplist_static[MAX_OPXMESG]


//Code





//Decode

void gen_oplist_static(uint32_t pinconf, uint8_t *oplist_static)
{
	int i=0,j=-1;
	while (i<MAX_OPXMESG)
	{
		if(((pinconf>>i)&1) == 1)
		{
			j++;
			oplist_static[j]=i;
		}
		i++;
	}
	j++;
	oplist_static[j]=END_OF_LIST;
}

void read_oplist_static(uint8_t *oplist_static)
{
	int i=0;
	
	while(oplist_static[i] != END_OF_LIST)
	{
		error_print("Pin %d\n", oplist_static[i]);
		i++;
	}
	
}


void decode_mesg(selector *sel, pinmsg *mesg)
{
	uint8_t set_oplist_static[MAX_OPXMESG];
	uint8_t clear_oplist_static[MAX_OPXMESG];
	
	gen_oplist_static(mesg->sbit, set_oplist_static);
	error_print("Printing the set oplist\n");
	read_oplist_static(set_oplist_static);
	gen_oplist_static(mesg->cbit, clear_oplist_static);
	error_print("Printing the clear oplist\n");
	read_oplist_static(clear_oplist_static);
	
	error_print("Running clear bit\n");
	set_pin_cycle(sel, clear_oplist_static, OFF);
	error_print("Running set bit\n");
	set_pin_cycle(sel, set_oplist_static, ON);
	
}


//Other

void print_message(pinmsg *to_read)
{
	error_print("Selector value: %d  setbit: %lu clearbit: %lu", to_read->slt, to_read->sbit, to_read->cbit);
}
