/** stepper motor driver
* io setup and library for controlling a stepper motor
* Dan Quinlivan <darknrgy@gmail.com>
 **/

#include <avr/interrupt.h>

// 0x05 = mhz/1024
#define STEPPER_TIMER_SCALAR 0x03;

enum { UP, DOWN };

struct stepper_struct{
    int16_t position;
    int16_t destination;
    uint16_t period;
    uint8_t phase;    
}   stepper;

// pin configuration for each step of the motor 
//[0] means no power to the motor
// two pins always active to increase torque
uint8_t poles_table[5] = {0x00, 0x03, 0x06, 0x0C, 0x09};


/** stepper_init
* initialize stepper driver
*/
void stepper_init(void){
    
    // use Port B as output
    DDRB = 0x0F;
        
    // give timer 1 a clock and scalar to chew on
    TCCR1B |= STEPPER_TIMER_SCALAR;    
    
 	TCCR1B |= _BV(WGM12);
    
    // initialize motor object
    stepper.position = 0;
    stepper.destination = 0;
    //stepper.period = 15625;
	stepper.period = 200;
    stepper.phase = 1;
    
    // initial period
    OCR1A = stepper.period;
	PORTB = 0xFF ^ poles_table[stepper.phase];
}

/** stepper_move_absolute
* tell the stepper to start moving to a new position and stop when it get there
* the new position will be equal to the param
**/
void stepper_move_absolute(uint16_t destination){
   stepper.destination = destination;
   TIFR |= _BV(OCF1A);
   TIMSK = 0x10;   
}

/** stepper_move_relative
* tell the stepper to start moving to a new position and stop when it gets there
* the new position will be the old position plus the param, negatives ok
**/
void stepper_move_relative(int16_t add_destination){
    stepper.destination += add_destination;
    TIFR |= _BV(OCF1A);
	TIMSK = 0x10;	
}

/** stepper_period
* change the speed of the stepper
**/
void stepper_period(uint16_t period){
	TCNT1 = 0;
	stepper.period = period;
}

/** interrupt on timer1 compare A
* step the motor, update position, update period
**/
ISR (TIMER1_COMPA_vect,ISR_NOBLOCK){
        
    // assign the new period to timer output compare A
    OCR1A = stepper.period;
    
    // if we are already at our destination, freeze the motor and pause the interrupts
    if (stepper.position == stepper.destination){
        // disable interrupt
		TIMSK = 0x00;
        return;
    } 

    // step the motor and update the position
    if (stepper.destination > stepper.position){
        if (++stepper.phase > 4) stepper.phase = 1;
        stepper.position ++;
    }else{
        if (--stepper.phase < 1) stepper.phase = 4;
        stepper.position --;
    }
    
    // assign pins to output
	PORTB = 0xFF ^ poles_table[stepper.phase];
    
}

