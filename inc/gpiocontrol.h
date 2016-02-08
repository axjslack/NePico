#ifndef _GPIOCONTROL_H
#define _GPIOCONTROL_H

#include <stdint.h>
#include <stdio.h>

#define PIN_SEL 8
#define PIN_SET_DIM 32
#define MAX_PIN 255

#define LOCALMAPPING


typedef enum gpiodirection {
	IN,
	OUT,
	ERR
} gpd;

typedef enum gpiovalue {
	OFF=0,
	ON=1
} gpv;

typedef uint8_t gpn;


typedef enum gpio_availability {
	OK,
	KO
} gpio_av; 


typedef struct single_pin {
	uint8_t gpion;
	gpd direction;
	gpio_av available;
} pin;

typedef struct pin_selector {
	pin pin_n[PIN_SET_DIM];
} selector;

//library function






typedef struct direct_pin {
		gpn gpion;
		gpd gpdir;
		gpv gpiov;	
} dpin;

/*typedef struct direct_pin_array {
	dpin pin_array[MAX_DIRECT_PIN];
} dpin_array;	
*/

#define PATH_BUF 128
#define LED_TIME 5
#define GPIO_KEY 44
#define GPIO_LED 45

int reserve_gpio(gpn n);
int set_direction(gpn n, gpd d);
gpv read_gpio(gpn n);
int write_gpio(gpn n, gpv v);
int poll_gpio(gpn n);
int led_pulse(gpn n);

uint8_t init_localpin(selector *localpin);
uint8_t multiled_kr_7p(selector *localpin);
void set_pin_cycle(selector *localpin, uint8_t *oplist_static, gpv op);


#endif // _GPIOCONTROL_H
