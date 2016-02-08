#ifndef _PINMESG_H
#define _PINMESG_H

#include "gpiocontrol.h" 


#define CMD_MASK 0x80
#define MAX_OPXMESG 32
#define END_OF_LIST 255


typedef struct pinmessage {
	uint8_t slt; //selector
	uint32_t sbit; //Set bit
	uint32_t cbit; //clear bit
} pinmsg;


/* Dinamic part temporaneally remvoed 
typedef struct operation_list_element {
	uint8_t op_pin;
	struct operation_list_element *next;
} oplist;
*/

void gen_oplist_static(uint32_t pinconf, uint8_t *oplist_static);
void read_oplist_static(uint8_t *oplist_static);
void decode_mesg(selector *sel, pinmsg *mesg);
void print_message(pinmsg *to_read);

#endif //_PINMESG_H
