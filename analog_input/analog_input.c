#include <avr/io.h>
#include <avr/interrupt.h>
#include "../../avr/lib/macros.h"
#include "../better_serial/usart.h"
#include <util/delay.h>
#include "../../avr/lib/analoginput.h"

/*
ISR(ADC_vect){

//	
	static float analog_value;
	//analog_value = analog_value * 0.98 + ((float) ADC * (5/1024.0f) * 1000 * 5.58) * 0.02;
	analog_value = analog_value * 0.9 + (ADC * (5/1024.0f) * 1000) * 0.1;
	usart_print_int32( (uint32_t) analog_value);
	usart_psend(PSTR("\n"));

}*/

int main(){
		
	usart_init();
	sei();
	analoginput_init();
	usart_psend(PSTR("hello\n"));

	/*
	// AVCC with external capacitor at AREF pin
	setbit(ADMUX, BIT(REFS0) | 0x02);	
	
	// set MUX bits, assming 0x00 for now...

	// ADC Enable
	setbit(ADCSRA, BIT(ADEN) | BIT (ADIE) | BIT(ADPS2) | BIT(ADPS1));// | BIT (ADPS0));

	*/

	float temp = 710;
	float battery_voltage = 12.0f;

	for(;;){
		// start a conversion
		//setbit(ADCSRA, BIT(ADSC));

		//temp = (float) analoginput_get(2);
		//temp = temp * (5/1024.0f) * 1000;
		battery_voltage = battery_voltage * 0.90 + (analoginput_get(1) * (5/1024.0f) * 1000 * 5.54) * 0.1;
		usart_print_int32( (uint32_t) battery_voltage);
		usart_psend(PSTR("\n"));

		
		_delay_ms(100);

	}
	

}



void usart_rx_callback(char *s){
	usart_psend(PSTR("Command Received: "));
	usart_send(s);
	usart_psend(PSTR("\n"));
}



