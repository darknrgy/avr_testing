#include <avr/io.h>

#include <util/delay.h>

int main (void){

	// startup delay
	_delay_loop_2(30000);

	int a = 100;
	int pins = 0x03;
	float delay = 10000;	

	// set portb to outputs
	DDRB = 0xFF;

	while (1){
		PORTB = 0xFF ^ pins;

		pins <<= 1;

		//if (pins > 8) pins = 1;
		if (((pins >> 4) & 1) == 1) pins |= 1;

		_delay_loop_2(delay);
		delay = delay * 0.999;
	}
}

	/*while(1){
		_delay_loop_2(20000);
		if ((PIND ^ 0xFF) & 0x01) a >>= 1;


		if ((PIND ^ 0xFF) & 0x02) a <<= 1;
		
		if (((PIND ^ 0xFF) & 0x04) != 0x00) a >>= 2;
		if (((PIND ^ 0xFF) & 0x08) != 0x00) a <<= 2;

		//a ++;
		if (a >= 0x40) a = 0x01;
		if (a <= 0x00) a = 0x40;
		PORTB = 0xFF ^ a;
		*/
