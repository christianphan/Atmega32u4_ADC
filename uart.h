#ifndef _AVRLIB_UART_H_
#define _AVRLIB_UART_H_
#include <stdint.h>
#include <avr/io.h>
#include <stdlib.h>



void uart_init();
void uart_newline(void);
void uart_putchar(char d);
void uart_print_pgm(const char *data);
void uart_putint(int32_t number, int8_t digits);
void uart_puthex(char c);
char uart_get_inbuf(void);
void uart_reset_inbuf(void);

#endif /* _AVRLIB_UART_H_ */

