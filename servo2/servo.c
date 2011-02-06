#define F_CPU 1000000UL  // 1 MHz
#define __OPTIMIZE__
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include "../macros/macros.h"


uint16_t testing;


#define SERVO_PIN 5
#define SERVO_PORT DDRC
#define SERVO_PORT_OUTPUT PORTC

struct servo_struct{

	char state;
	uint16_t period;	
	uint16_t pulse_width;
	uint16_t next_event;
	uint16_t event_counter;
} servo;

void testing_function(){
	servo.pulse_width = servo.pulse_width + 5;
	if (servo.pulse_width >= 250){ servo.pulse_width = 125;}
	testing = servo.pulse_width;
}



int main( void ){

	servo_init();
    sei();
	setbit(DDRB, 0xFF);

    do{
		testing_function();
		flipbit(PORTB, 1);
		_delay_ms(100);
        // sleep_mode();
    }while(1);
	
}



void servo_init(){
	
	// intialize servo object
	servo.state = 0x00;
	servo.period = 2500;
	servo.pulse_width = 125;
	servo.next_event = 10;
	servo.event_counter = 0;

	// initialize port/pin for output
	setbit(SERVO_PORT, BIT(SERVO_PIN));
		
	// set mode to pwm fast mode
	setbit(TCCR2, BIT(WGM20) | BIT(WGM21));

	// set output compare value (128)
	OCR2 = 0;

	// set interrupt for output compare match and overflow
	setbit(TIMSK, BIT(TOIE2) | BIT(OCIE2));
	
	// set scalar mode, kick off timer
	setbit(TCCR2, BIT(CS21));
	

}




void servo_event(){
	
	
	// reset event counter
	servo.event_counter = 0;
	
	// reset timer2
	//TCNT2 = 0x00;

	if (servo.state == 0x00){
		
		// turn on servo
		servo.state = 0x01;
		setbit(SERVO_PORT_OUTPUT, BIT(SERVO_PIN));

		// calculate next event
		servo.next_event = floor ((servo.pulse_width + OCR2) / 256);

		OCR2 = (char) ((servo.pulse_width + OCR2) % 256);

		return;

	}

	if (servo.state == 0x01){

		// turn off servo
		servo.state = 0x00;
		clearbit(SERVO_PORT_OUTPUT, BIT(SERVO_PIN));

		// calculate next event
		servo.next_event = floor((servo.period - servo.pulse_width + OCR2) / 256);
		OCR2 = (servo.period - servo.pulse_width + OCR2) % 256;
		
		return;
	}	
}

void servo_interrupts(char type){

	// overflow interrupt
	if (type == 1){
		servo.event_counter ++;
		return;
	}

	// compare interrupt
	if (servo.event_counter >= servo.next_event){
		servo_event();
		return;			
	}	
	return;
}


// interrupt on OCR2 compare match
ISR (TIMER2_COMP_vect){

	servo_interrupts(0);
}


// interrupt on timer2 overflow
ISR (TIMER2_OVF_vect){

	servo_interrupts(1);
}


