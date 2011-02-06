#define F_CPU 1000000UL  // 1 MHz
#define __OPTIMIZE__
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "../macros/macros.h"
#include "../better_serial/usart.h"
#include "../analog_input/analog_input.h"

int main(){

	analog_input_init();
	usart_init();
    sei();

	uint8_t v;

	for(;;){

		v = ADCH;
		print_int_to_serial(v);
		_delay_ms(100);
	}


}


uint8_t usart_rx_callback(char *s){
	// do nothing
	return 0;
}


void print_int32_to_serial(uint32_t v){

	char str[64] = "";
	sprintf(str, "value is %" PRId32 "\n", v);
	usart_send(str);	
	
}

void print_int_to_serial(uint8_t v){

	char str[8] = "";
	sprintf(str, "%" PRIo8 "\n", v);
	usart_send(str);	
	
}
