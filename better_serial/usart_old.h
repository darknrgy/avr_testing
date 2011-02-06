#define USART_BAUD 9600
#define MYUBBR F_CPU/8/USART_BAUD-1

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

static char rxbuff[32];
uint8_t position;

/* usart_rx_callback
 * define this function yourself
 * once a \r is sent over usart, this function will
 * be called with the string from usart rx
 */
uint8_t usart_rx_callback(char *s);

/* usart_init
 * initialize usart for tx and rx
 */
void usart_init(){

	uint16_t ubbr = MYUBBR;

	UBRRH = (unsigned char) (ubbr>>8);
	UBRRL = (unsigned char) ubbr;
	
	// use double transmission speed (works better on 1mhz)
	UCSRA |= _BV(U2X);	

	// Enable receiver and transmitter
	UCSRB |= _BV(RXCIE) | _BV(RXEN) | _BV(TXEN);
}

/*
 * Send character c down the UART Tx, wait until tx holding register
 * is empty.
 */
static void usart_putchr(char c){
  loop_until_bit_is_set(UCSRA, UDRE);
  UDR = c;
}

/*
 * Send a C (NUL-terminated) string down the UART Tx.
 */
static void usart_send(const char *s){
	while (*s){
		if (*s == '\n') usart_putchr('\r');
      	usart_putchr(*s++);
    }
}

/*
 * Same as above, but the string is located in program memory,
 * so "lpm" instructions are needed to fetch it.
 */
static void usart_psend(const char *s){
    char c;
    for (c = pgm_read_byte(s); c; ++s, c = pgm_read_byte(s)){
        if (c == '\n') usart_putchr('\r');
        usart_putchr(c);
    }
}

/*
 * UART receive interrupt.  Fetch the character received and buffer
 * it, unless there was a framing error..
 */
ISR(USART_RXC_vect){
	uint8_t c;
    c = UDR;
    if (bit_is_clear(UCSRA, FE)){
        if (c == '\r'){
			rxbuff[position] = 0x00;
			position = 0;
			usart_rx_callback(rxbuff);
			return;
		}
		position ++;
		if (position >= 32){
			usart_psend(PSTR("Receieve Buffer Full\n"));
		}
		rxbuff[position-1] = c;		
	}else{
		position = 0;
		usart_psend(PSTR("Framing Error Detected\n"));
	}
}
