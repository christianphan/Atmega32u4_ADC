/*
 * lab6.c
 *
 * Created: 5/30/2017 9:44:02 PM
 * Author : chris
 */

 #include <string.h>
#include <avr/io.h>
#include <stdlib.h>
#include "uart.h"
#include "nrf.h"
#include "spi.h"

 #define F_CPU 16000000
 #define BUAD 9600
 #define BRC ((F_CPU/16/BUAD) - 1)
 #include <util/delay.h>


/*
 * ATmega32u4  * ATmega32u4 breakout Pinout
 *   1  /Reset
 *   2  PD0 (Unused)
 *   3  PD1 UART TXD
 *   4  PD2 UART RXD
 *   5  PD3 UART TXD
 *   6  PD4 (unused)
 *   7  VCC
 *   8  GND
 *   9  PB6 (unused)
 *  10  PB7 (unused)
 *  11  PD5 (unused)
 *  12  PD6 OC0A PWM
 *  13  PD7 (unused)
 *  14  PB0 BLE SS/REQN
 *
 *  15  PB1 BLE SCLK/SCK
 *  16  PB2 BLE MOSI
 *  17  PB3 BLE MISO
 *  18  PB4 BLE RDYN
 *  19  PB5 BLE RST
 *  20  AVCC
 *  21  AREF
 *  22  GND
 *  23  PC0 (unused)
 *  24  PC1 (unused)
 *  25  PC2 (unused)
 *  26  PC3 (unused)
 *  27  PC4 (unused)
 *  28  PC5 (unused)
 *  29  PF0 (unused)
 *  30  PF1 ADC1
 */




char buffer[20];


unsigned char GetAdc_value(void)
{
	ADCSRA |= (1<<ADSC);	// Start A to D conversion
	while (ADCSRA & 1<<ADSC);	// Wait for conversion to complete
	return (ADCH);
}

void uart_sendstr(char * str)
{
	while(*str)
	{
		uart_putchar(*str++);

	}
	uart_putchar(' ');
}

void ADCsetup()
{

	ADMUX = (1 << MUX0) | (1<<ADLAR); //reads adc1
	ADCSRA = (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
	DIDR0 = (1 << ADC5D); //disables the digital input buffer

}

void sendNRF()
{

}

int main(void)
{
	int8_t ret;
	uart_init();
	ADCsetup();
	spi_init();
	nrf_tx_map_pipes(); //might not work
	ret = nrf_setup(); //this part creates error. transmit part
	uart_putint(ret, 1);  //figure out what this does

	while (1)
	{

		uart_putchar('k');
		char adc_str[16];
		unsigned char adc_value = '0';
		adc_value =	GetAdc_value();

		//converts adc_value
		itoa(adc_value, adc_str, 10);
		uart_sendstr(adc_str);
		//nrf_print_temperature();
		_delay_ms(500);

	}
	return 0;
}







