/** serial port IO
 * functions for communicating over UART
 **/

#define F_CPU 1000000UL	/* CPU clock in Hertz */

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

// improves baud rate error @ F_CPU = 1 MHz on some chips?
//UCSRA = _BV(U2X);




/*
 * Send character c down the UART Tx, wait until tx holding register
 * is empty.
 */
static void putchr(char c){
  loop_until_bit_is_set(UCSRA, UDRE);
  UDR = c;
}

/*
 * Send a C (NUL-terminated) string down the UART Tx.
 */
static void printstr(const char *s){
	while (*s){
		if (*s == '\n') putchr('\r');
      	putchr(*s++);
    }
}

/*
 * Same as above, but the string is located in program memory,
 * so "lpm" instructions are needed to fetch it.
 */
static void printstr_p(const char *s){
	char c;

  	for (c = pgm_read_byte(s); c; ++s, c = pgm_read_byte(s)){
		if (c == '\n') putchr('\r');
      	putchr(c);
    }
}

int main(void){

  UCSRA = _BV(U2X);		/* improves baud rate error @ F_CPU = 1 MHz */
  UCSRB = _BV(TXEN)|_BV(RXEN)|_BV(RXCIE); /* tx/rx enable, rx complete intr */
  UBRRL = (F_CPU / (8 * 9600UL)) - 1;  /* 9600 Bd */
  uint16_t number = 0;
  char err[256] = "testing 123 fuck you\n";
	
	while(1){
		//_delay_loop_2(30000);

		number ++;


		
		
		//printstr_p(PSTR("DAN RULZ THE WORLD\n"));

		sprintf(err, "%d, ", number);
		printstr(err);
		if (number % 10 == 0) printstr_p(PSTR("\n"));
	}

}
