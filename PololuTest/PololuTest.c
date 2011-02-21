#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/interrupt.h>

//#include "../../avr/lib/pwm.h"

int main(){

	DDRC = 0xDB;
	PORTC = 0x7E;;
	

	
	sei();
	pwm_init();
	pwm_set_scalar(3);
	pwm_set(0x00, 120);
	pwm_set(0x01, 255);
	
	int a;
	while(1){
		a++;
	}

}

	
