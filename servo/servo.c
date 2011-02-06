#define F_CPU 16000000UL  // 1 MHz
// #define __OPTIMIZE__
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include "../macros/macros.h"



#define SERVO_PIN 5
#define SERVO_PORT DDRC
#define SERVO_PORT_OUTPUT PORTC

#define SERVO_COUNTER_OFFSET 115;

int main( void ){
		
	int rand_num;
	servo_init();
    sei();
    for(;;){
				
		rand_num = rand_range(0,3);

		// 220 = top
		// 100 = bottom
		if (rand_num != 0) OCR2 = 210;
		else OCR2 = 175;
				
		_delay_ms(50);        
    }
}

int rand_range(a, b){
	int t = rand();
	t = t % (b-a+1);
	t += a;
	return t;
}

void servo_init(){
	
	// initialize port/pin for output
	setbit(SERVO_PORT, BIT(SERVO_PIN));
		
	// set mode to phase correct pwm
	setbit(TCCR2, BIT(WGM20));

	// set output compare value (128)
	OCR2 = 240;

	// set interrupt for output compare match and overflow
	setbit(TIMSK, BIT(TOIE2) | BIT(OCIE2) ); //
	
	// set scalar mode, kick off timer
	//setbit(TCCR2, BIT(CS21));
	setbit(TCCR2, BIT(CS20) | BIT(CS22));
	
	

}


// interrupt on OCR2 compare match
ISR (TIMER2_COMP_vect){

	servo_interrupts(0);
}


// interrupt on timer2 overflow
ISR (TIMER2_OVF_vect){

	servo_interrupts(1);
}

void servo_interrupts(char type){

	static uint16_t i = 0;
	static char state = 0x00;
	
	// compare interrupt
	if (type == 0){
		
		if (i != 0) return;
		if (state == 0x01){
			setbit(SERVO_PORT_OUTPUT, BIT(SERVO_PIN));
			state = 0x00;

		}else{
			clearbit(SERVO_PORT_OUTPUT, BIT(SERVO_PIN));
		}
		return;
	}

	if (i == 0){
		// enable interrupts for compare match
		//setbit(TIMSK, BIT(OCIE2));
		state = 0x01;
	}else{
		// disable interrupts for compare match
		//clearbit(TIMSK, BIT(OCIE2));
	}
			
	i ++;
	if (i >= 4) i = 0;

	return;
}
		


/*

void servo_interrupts(char type){

	static char i = 0;
	
	// overflow interrupt
	if (type == 1){
		
		if (i == 0){
			if (OCR2 < SERVO_COUNTER_OFFSET){
				temp = OCR2;
			}
			
			setbit(SERVO_PORT_OUTPUT, BIT(SERVO_PIN));
			clearbit(TIMSK, BIT(TOIE2));
			setbit(TIMSK, BIT(OCIE2));
		}
		
		i ++;
		if (i == 10) i = 0;		
		return;
	}

	// compare interrupt
	if (i == 0){
		if (OCR2 <= SERVO_COUNTER_OFFSET){ 
			i ++;
			// ignore interrupt
			return;
		}else{
			clearbit(SERVO_PORT_OUTPUT, BIT(SERVO_PIN));
			setbit(TIMSK, BIT(TOIE2));
			clearbit(TIMSK, BIT(OCIE2));
			i ++;
			return;	
		}
	}
	clearbit(SERVO_PORT_OUTPUT, BIT(SERVO_PIN));
	setbit(TIMSK, BIT(TOIE2));
	clearbit(TIMSK, BIT(OCIE2));
	return;
}
*/

/*
void servo_interrupts(char type){

	static char i = 0;

	// overflow interrupt
	if (type == 1){
		i ++;
		if (i == 10) i = 0;
		clearbit(SERVO_PORT_OUTPUT, BIT(SERVO_PIN));
		return;
	}

	// compare interrupt
	if (i == 1){
		setbit(SERVO_PORT_OUTPUT, BIT(SERVO_PIN));
	}
	return;
}
*/








