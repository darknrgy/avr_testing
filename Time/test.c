#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#include <util/delay.h>


#include "../lib/time.h"
#include "../lib/macros.h"
#include "../lib/usart.h"



#define INTERRUPT_COUNTER_MAX 1000000000

typedef struct InterruptCounterResult{
	uint32_t delta;
	uint32_t previous;
} InterruptCounterResult;


uint32_t* interrupt_counter_1();
InterruptCounterResult interrupt_counter_delta(uint32_t previous);




int main( void ){

	DDRB = 0xFF;

	TimeResult tr;
	InterruptCounterResult icr; 

	double time;
	double ips = 0;

	tr.previous = 0;
	icr.previous = 0;	

	time_init();
	usart_init();
	
	// set INT0 external interrupt, riding edge
	setbit(MCUCR, BIT(ISC01) | BIT(ISC00));
	setbit(GICR, BIT(INT0));
	
	sei();
	

	
	uint8_t a = 0;
	
	while (1){
		a ++;
		_delay_ms(20);		
		tr = time_get_time_delta(tr.previous);
		icr = interrupt_counter_delta(icr.previous);
		time = get_time_in_ms(tr.delta);
		ips = ips * 0.65 + ((double) icr.delta /(double) 16 / (double) time * (double) 1000 * (double) 60) * 0.35;
		//ips = ((double) icr.delta /(double) 16 / (double) time * (double) 1000 * (double) 60);
		
		if (a == 1){
			usart_print_int32((uint32_t) ips);
			a = 0;
		}
	}

}


ISR(INT0_vect, ISR_NOBLOCK){
	
	uint32_t* counter = interrupt_counter_1();
    *counter = *counter + 1;
    if (*counter >= (uint32_t) INTERRUPT_COUNTER_MAX){
		*counter = 0;
	}
	flipbit(PORTB, 1);

}

uint32_t* interrupt_counter_1(){
	static uint32_t counter;
	return &counter;

}


InterruptCounterResult interrupt_counter_delta(uint32_t previous){

    InterruptCounterResult result;
    uint32_t* current = interrupt_counter_1();
    if (*current >= previous) result.delta = *current - previous;
    else result.delta = (uint32_t) INTERRUPT_COUNTER_MAX - previous + *current;
    result.previous = *current;
    return result;
}

void usart_rx_callback(char* s, uint8_t length){

	usart_send_by_length(s, length);
}

