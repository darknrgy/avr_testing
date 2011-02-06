/*
#define __OPTIMIZE__
#include <stdlib.h>
#include <util/delay.h>
#include <string.h>
*/

#define F_CPU 1000000UL  // 1 MHz
#define __OPTIMIZE__
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h> 
#include <avr/interrupt.h>
#include <inttypes.h> 
#include "../macros/macros.h"
#include "clock.h"
#include "../better_serial/usart.h"



void print_int32_to_serial(uint32_t v){

	char str[64] = "";
	sprintf(str, "value is %" PRId32 "\n", v);
	usart_send(str);
	usart_psend(PSTR("hello\n"));
	
}


int main(){

	// init timer~
	unixclock_init();

	// set time
	unixclock_settime(1233740160);

	sei();

	usart_init();

	

	uint32_t a = 0;
	// uint32_t
	uint32_t timer2 = 0;

	while(1){
		
		timer2 = unixclock_gettime();
		print_int32_to_serial(timer2);
		_delay_ms(20000);
	}
}


uint8_t usart_rx_callback(char *s){
	// do nothing
	return 1;
}





