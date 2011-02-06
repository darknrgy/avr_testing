#include <util/delay_basic.h>
#include "../better_serial/usart.h"
#include "../stepper/stepper.h"
void long_pause (uint16_t length);

int main( void ){

	// initialize usart
	usart_init();
	usart_psend(PSTR("usart initialized, initializing stepper motor... "));

	// initialize stepper motor
	stepper_init();
	usart_psend(PSTR("done\n"));
	
	// enable interrupts
	sei();
	
	// loop forever
	while (1){	
		sleep_mode();
	}	
}























void long_pause (uint16_t length){

	uint16_t i;
	length *= 3;

	for (i = 0; i <= length; i ++){
		_delay_loop_2(0);		
	}
}
