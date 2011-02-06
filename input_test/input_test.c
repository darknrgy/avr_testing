#define F_CPU 1000000UL  // 1 MHz
//#define __OPTIMIZE__
#include <avr/io.h>
#include <util/delay.h>
#include "../macros/macros.h"
#include "../better_serial/usart.h"


int main (void)
{
	char state = 0x00;	

	// set up port b for output
	DDRB = 0xFF;

	// set up porn c for input
	DDRC = 0x00;

	// enable pullup resistors
	PORTC = 0xFE; 

	state = PINC;

	// init usart
	usart_init();	

	sei();

	// PORTC = 0xFF; 
	while (1){

		_delay_ms(100);
		if (getbit(PINC, 0x01)){
			usart_psend(PSTR("ON\n"));
		}else{
			usart_psend(PSTR("OFF\n"));
		}
	
		PORTB = PINC;
	}
} 

uint8_t usart_rx_callback(char *s){
	usart_psend(PSTR("Command Received: "));
	usart_send(s);
	usart_psend(PSTR("\n"));
	return 1;
}
