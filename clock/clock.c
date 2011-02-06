/*
#define __OPTIMIZE__
#include <stdlib.h>
#include <util/delay.h>
#include <string.h>
*/

#define F_CPU 1000000UL  // 1 MHz
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "../macros/macros.h"
#include "../better_serial/usart.h"


// uint32_t
char timer2 = 0;

int main(){

	// init timer
	unixclock_init();

	// set time
	unixclock_settime(1233557989);

	// init usart
	usart_init();	

	sei();

	

	char a = 0;

	while(1){
		
		timer2 = unixclock_gettime();
		_delay_ms(250);
		a ++;
		PORTB = a;
	}
}





