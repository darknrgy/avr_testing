#include <stdio.h>
#include <util/delay.h>
#include "../macros/macros.h"
#include "usart.h"
//#include "../stepper/stepper.h"

void print_int32_to_serial(uint32_t v){

	char str[64] = "";
	sprintf(str, "%" PRId32, v);
	usart_send(str);	
	
}

int main( void ){

	// initialize usart	
	usart_init();
	sei();
	uint32_t i = 0;
	for(;;){
		//usart_psend(PSTR("Testing"));
		//print_int32_to_serial(i);
		//usart_psend(PSTR("\n"));
		//i++;
		_delay_ms(25);
		

	}
}

void usart_rx_callback(char *s){
	usart_psend(PSTR("Command Received: "));
	usart_send(s);
	usart_psend(PSTR("\n"));
}




void print_int_to_serial(uint8_t v){

	char str[8] = "";
	sprintf(str, "%" PRIo8, v);
	usart_send(str);	
	
}
