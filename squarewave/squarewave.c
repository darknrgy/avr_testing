#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "usr/iocompat.h" 

float frequency = 2000;
float up;
float down;
float pulsewidth;

void
ioinit (void)                   /* Note [6] */
{
    /* Timer 1 is 10-bit PWM (8-bit PWM on some ATtinys). */
    
	// this thingy makes pin 1 start doing pwm
	//TCCR1A = TIMER1_PWM_INIT;
    /*
     * Start timer 1.
     *
     * NB: TCCR1A and TCCR1B could actually be the same register, so
     * take care to not clobber it.
     */
	 // need this
    TCCR1B |= TIMER1_CLOCKSOURCE;
    /*
     * Run any device-dependent timer 1 setup hook if present.
     */
#if defined(TIMER1_SETUP_HOOK)
    //TIMER1_SETUP_HOOK();
#endif

    /* Set PWM value to 0. */
    //OCR = 0;

    /* Enable OC1 as output. */
    //DDROC = _BV (OC1);

    /* Enable timer 1 overflow interrupt. */
    // need these things
	//TIMSK = _BV (TOIE1);
   // sei ();
}

int main (void){

	ioinit();
	
	uint8_t pins = 0x00;
	
	// set portb to outputs
	DDRB = 0xFF;

	// set portd to inputs
	DDRD = 0x00;

	// set portb to pins
	PORTB = 0xFF ^ pins;


	pulsewidth = 0.5;
	recalculate();
	

	while (1){				
		// toggle the pin
		pins ^= 0x02;
		
		// assign pins to output
		PORTB = 0xFF ^ pins;

		_delay_loop_2((int) up);

		// toggle the pin
		pins ^= 0x02;
		
		// assign pins to output
		PORTB = 0xFF ^ pins;

		_delay_loop_2((int) down);

		if ((PIND ^ 0xFF) & 0x01){
			pulsewidth += 0.001;
			
			//i_frame = 0;
		}

		if ((PIND ^ 0xFF) & 0x02){
			pulsewidth -= 0.001;
			
		//	i_frame = 0;
		}
		recalculate();

	}
}

/** recalculate
* calculate new values
**/
void recalculate(){
	
	if (pulsewidth > 1) pulsewidth = 1;
	if (pulsewidth < 0) pulsewidth = 0;
	up = frequency * pulsewidth;
	down = frequency - up;
}

/** interrupt for countdown timer
**/
/*
ISR (TIMER1_OVF_vect){

	//static uint16_t i_frame = 0;
	//i_frame += 1;
	//if (i_frame >= 2){
		if ((PIND ^ 0xFF) & 0x01){
			pulsewidth += 0.001;
			recalculate();
			//i_frame = 0;
		}

		if ((PIND ^ 0xFF) & 0x02){
			pulsewidth -= 0.001;
			recalculate();
		//	i_frame = 0;
		}
	//}
	//if (i_frame > 2) i_frame = 2;
}*/





