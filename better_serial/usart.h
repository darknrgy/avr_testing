#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>
#include "../macros/macros.h"


// automagically #define macros for usart configuration
#define BAUD 57600
#include <util/setbaud.h>


static char usart_rxbuff[32];
uint8_t usart_position;

/* usart_rx_callback
 * define this function yourself
 * once a \r is sent over usart, this function will
 * be called with the string from usart rx
 */
void usart_rx_callback(char *s);

/* usart_init
 * initialize usart for tx and rx
 */
void usart_init(){
    
    UBRRH = UBRRH_VALUE;
    UBRRL = UBRRL_VALUE;
    #if USE_2X
    setbit(UCSRA, BIT(U2X));
    #else
    clearbit(UCSRA, BIT(U2X));
    #endif

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
    
    if (!bit_is_clear(UCSRA, FE)){
 		usart_position = 0;
		usart_psend(PSTR("!framing_error\n"));
        return;
    }

	uint8_t c;
    c = UDR;
    
    if (c == '\r'){
        usart_rxbuff[usart_position] = 0x00;
        usart_position = 0;
        usart_rx_callback(usart_rxbuff);
        return;
    }
    
    usart_position ++;
    
    if (usart_position >= 32){
		usart_position = 0;
        usart_psend(PSTR("!buffer_full\n"));
    }
    
    usart_rxbuff[usart_position-1] = c;		
	
}


void usart_print_int32(uint32_t v){

	char str[64] = "";
	sprintf(str, "%" PRIi32, v);
	usart_send(str);	
	
}
