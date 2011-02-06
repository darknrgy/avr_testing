#include "../macros/macros.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "../better_serial/usart.h"




// initialize PWMMC

void timer_init(){
	
	// set  PORTB to output mode
	setbit(DDRB, 0xFF);
    
    // set initial values
    OCR1A = 0x100; 

	PORTB = 0;
    
    // enable interrupt on overflow to do shit
    //setbit(TIMSK, BIT(OCIE1B)); // BIT(TOIE1)
    
    // initialize timer
    setbit(TCCR1A, BIT(COM1A1)| BIT(WGM11) | BIT(WGM11) | BIT(WGM10));
	
	// set scalar and kick off timer
    setbit(TCCR1B, BIT(CS11));	

}


void usart_rx_callback(char *s){
	
	uint16_t newtime;
	
	usart_psend(PSTR("Command Received: "));
	usart_send(s);
	usart_psend(PSTR("\n"));
	
	newtime = atol(s);
	OCR1A = newtime;
	
	
	
}


int main(){

	timer_init();
	usart_init();
	sei();
	usart_psend(PSTR("EAT A DICK\n"));
	while (1){}

}
