#ifndef USART_H
#define USART_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <string.h>
#include "../lib/macros.h"

#include <util/setbaud.h>


/* usart_init
 * initialize usart for tx and rx
 */
void usart_init();


/*
 * Send character c down the UART Tx, wait until tx holding register
 * is empty.
 */
void usart_putchr(char c);

/*
 * Send a C (NUL-terminated) string down the UART Tx.
 */
void usart_send(const char *s);

/*
 * Same as above, but the string is located in program memory,
 * so "lpm" instructions are needed to fetch it.
 */
void usart_psend(const char *s);

/* 
 * Send an unsigned 32 bit as a number string 
 */
void usart_print_int32(uint32_t v);

/* 
 * Send an unsigned 32 bit as a hex string 
 */
void usart_print_int32_hex(uint32_t v);


/* usart_rx_callback
 * define this function yourself
 * once a \r is sent over usart, this function will
 * be called with the string from usart rx
 */
void usart_rx_callback(char *s);

#endif // #ifndef USART_H

