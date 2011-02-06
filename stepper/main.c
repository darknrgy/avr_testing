#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include "stepper.h"
#include <util/delay_basic.h>

int main (void){
    
	wdt_disable();
	stepper_init();
    sei();
	while (1){
		stepper_period(40);
		stepper_move_relative(+96);    
		long_pause(1);
		stepper_period(500);
		stepper_move_relative(-96);    
		long_pause(4);
	}
}

void long_pause (uint16_t length){

	uint16_t i;
	length *= 3;

	for (i = 0; i <= length; i ++){
		_delay_loop_2(0);
	}
}

