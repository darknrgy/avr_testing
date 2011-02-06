
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../macros/macros.h"

#define ML_RANGE 1380




// initialize PWMMC

void timer_init(){
	
	// set  PORTB to output mode
	setbit(DDRB, 0xFF);
    
    // set initial values
    OCR1A = 0xFFE; 
    
	//OCR1B = 0x300; 
    OCR1B = ML_RANGE * 2 / 3 + 5;
	PORTB = 0;
    
    // enable interrupt on overflow to do shit
    setbit(TIMSK, BIT(OCIE1B)); // BIT(TOIE1)
    
    // initialize timer
    // 8 bit PWM non-inverted
	setbit(TCCR1A, BIT(COM1A1)| BIT(WGM11) | BIT(WGM10));
	setbit(TCCR1B, BIT(WGM12));
	
	// TCCR1A = 0xA1;
	
    // set scalar and kick off timer
    setbit(TCCR1B, BIT(CS10));
	//setbit(TCCR1B, 0x02);  

}



int32_t color[3] = {0,0,0};
int32_t offsets[3] = {0, ML_RANGE * 2 / 3 , ML_RANGE * 4 / 3 };


int32_t counter(int32_t increment ){
	
	static int32_t counter = 0;
	static int32_t delayer = 0, delayer2 = 0, delayer3 = 0;

	delayer ++;
	if (delayer > 17){
		counter += increment;
		if (counter > 65536) counter = 0;
		delayer = 0;
	}
	delayer2 ++;
	if (delayer2 >=36){
		offsets[1] += 1;
		delayer2 = 0;
	}

	delayer3 ++;
	if (delayer3 > 52){
		offsets[2] += 1;
		delayer3 = 0;
	}

	
	return counter;

}

ISR(TIMER1_COMPB_vect){
	PORTB = 0x00;

	static int phase = 0;

	
	switch (phase){
		case 0: 
			setbit(PORTB, BIT(3)); break;
		case 1: 
			setbit(PORTB, BIT(4)); break;
		case 2: 
			setbit(PORTB, BIT(5)); break;
	}


	OCR1A = color[phase];

	


	color[phase] =  abs(( (counter(1) + offsets[phase]) % (ML_RANGE * 2)) - ML_RANGE) - ML_RANGE/3;
	if (color[phase] < 0) color[phase] = 0;



	phase ++;
	if (phase > 2) phase = 0;
	return;
}





int main(){
	
	char fuck;
	//setbit(DDRC, 0xFF);
	sei();
	timer_init();
	for(;;){
		
		fuck = 'a';
		_delay_ms(1000);
		//PORTC = 0xFF;
		_delay_ms(1000);
		//PORTC = 0x00;


		//counter(10);
	}
	timer_init();
}


