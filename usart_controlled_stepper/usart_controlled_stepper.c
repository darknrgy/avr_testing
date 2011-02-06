#include <util/delay_basic.h>
#include <string.h>
#include <stdlib.h>
#include "../better_serial/usart.h"
#include "../stepper/stepper.h"

void long_pause (uint16_t length);

int main( void ){

	// initialize usart
	usart_init();
	usart_psend(PSTR("\n\nSerial Port Stepper Motor Driver 1.0\n"));
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

uint8_t usart_rx_callback(char *s){

	const char *delimiters = " ";
	char *command;
	char *param;
	int16_t params[6];
	uint8_t i = 0;

	command = strsep(&s, delimiters);
	while (s != 0x00 && i < 6){
		param = strsep(&s, delimiters);
		params[i] = atoi(param);
		i++;
	}

	if (strcmp(command, "abs") == 0){
		stepper_move_absolute(params[0]);		
		usart_psend(PSTR("abs pos defined\n"));
		return 1;
	}

	if (strcmp(command, "rel") == 0){
		stepper_move_relative(params[0]);		
		usart_psend(PSTR("rel pos defined\n"));
		return 1;		
	}

	if (strcmp(command, "per") == 0){
		stepper_period(params[0]);		
		usart_psend(PSTR("period defined\n"));
		return 1;		
	}	

	usart_psend(PSTR("command not found\n"));	

	return 0;
}























void long_pause (uint16_t length){

	uint16_t i;
	length *= 3;

	for (i = 0; i <= length; i ++){
		_delay_loop_2(0);		
	}
}
