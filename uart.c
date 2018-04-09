/*
 * uart.c
 *
 *  Created on: Mar 15, 2018
 *      Author: chris
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "uart.h"
#define BRC ((16000000/16/9600) - 1)

void uart_init()
{
	UBRR1H = (unsigned char) (BRC >> 8);
	UBRR1L = BRC;

	//enable recieve and transmit
	UCSR1B = ( 1 << TXEN1 )| (1<<RXEN1);

	//set frame format
	UCSR1C = ( 1 << USBS1 ) | ( 3 << UCSZ10 );

}


/**
 * Converts a given number digit by digit to a character buffer.
 * Note: the following limits are applied:
 * <ul>
 * <li>negative numbers are converted to positive numbers</li>
 * <li>base 10 conversion only</li>
 * <li>maximum number of 9 digits are converted</li>
 * </ul>
 *
 * @param number Number to be converted.
 * @param buf    Buffer the digits are put.
 * @return Number of digits converted.
 */
static uint8_t
tobuf(int32_t number, char *buf)
{
	int32_t next_digit;
	int8_t i;

	if (number < 0) {
		number *= -1;
	}

	for (i = 0; i < 10 && number; i++) {
		next_digit = (int32_t) (number / 10);
		buf[i] = ((char) (number - next_digit * 10)) + '0';
		number = next_digit;
	}
	return --i;
}



//uart_transmit
void uart_putchar(char value)
{
	// wait for empty transmit buffer
		while ( ! ( UCSR1A & ( 1 << UDRE1 ) ) );
		// put data into buffer, sends data
		UDR1 = value;
}


void uart_newline(void)
{
	uart_putchar('\r');
	uart_putchar('\n');
}

void uart_print_pgm(const char *data)
{
    while (pgm_read_byte(data) != 0x00) {
        uart_putchar(pgm_read_byte(data++));
    }
}


/**
 * Prints a given signed base 10 number via UART.
 * A number of minumum digits can be specified. If the given number has
 * less digits, the output is filled with leading zeros. If the number
 * has more digits, all digits are printed.
 * Note: this function uses the number_to_buf() method, therefore the
 * same limits apply here.
*
 * @param number Number to be printed.
 * @param digits Minimum number of digits to print.
 */
void
uart_putint(int32_t number, int8_t digits)
{
	char buf[10];
	int8_t i;

	if (number < 0) {
		uart_putchar('-');
		number *= -1;
	}

	i = tobuf(number, buf);

	while (i < --digits) {
		uart_putchar('0');
	}
	while (i >= 0) {
		uart_putchar(buf[i--]);
	}
}


static const char hexvals[] = "0123456789abcdef";
void
uart_puthex(char c)
{
    uart_putchar(hexvals[(c >> 4) & 0x0f]);
    uart_putchar(hexvals[c & 0x0f]);
}



