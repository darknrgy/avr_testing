#include <avr/interrupt.h>
#include <avr/io.h>
#include "../lib/macros.h"



int main( void ){

			
	float time = 0;
	sei();
	DDRB = 0xFF;
	setbit(MCUCR, BIT(ISC01) || BIT(ISC00));
	setbit(GICR, BIT(INT0));
	while (1){

	}

}

void test(uint8_t a){
	a = 2;
}


ISR(INT0_vect){
	
	flipbit(PORTB, 1);

}
