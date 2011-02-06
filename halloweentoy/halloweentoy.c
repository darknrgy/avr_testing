#define F_CPU 1000000UL  // 1 MHz
//#define __OPTIMIZE__
#include <avr/io.h>
#include <util/delay.h>
#include "../macros/macros.h"


int main (void)
{
	// set up port b for output
	DDRB = 0xFF;

	// set up porn c for input
	DDRC = 0x00;

	// enable pullup resistors
	PORTC = 0xFE; 

	// PORTC = 0xFF; 
	while (1){

		
		if (getbit(PINC, 0x01)){
			// PORTB = something
			activate();

			
		}
		
	}
} 

void  activate(){
	
	// turn flash on
	setbit(PORTB, 1);
	

	// buzz
	_delay_ms(390);

	for(uint16_t i = 0; i <= 10; i++){
		PORTB = 0x00;
		_delay_ms(10);
		PORTB = 0x02;
		_delay_ms(6);
		
	}


	// turn flash off
	clearbit(PORTB, 1);
	_delay_ms(2000);

}





