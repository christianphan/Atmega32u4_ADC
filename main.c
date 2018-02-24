/*
 * lab6.c
 *
 * Created: 5/30/2017 9:44:02 PM
 * Author : chris
 */

 #include <string.h>
#include <avr/io.h>

 #define F_CPU 16000000
 #define BUAD 9600
 #define BRC ((F_CPU/16/BUAD) - 1)
 #include <util/delay.h>

#define LED_SET 1 << 6
// And a bitmask to clear the bit
#define LED_CLEAR ~(LED_SET)


int main(void)
{

    DDRE |= LED_SET;
    // Turn the LED off.
    PORTE &= LED_CLEAR;

	uart_init();

	while (1)
	{
		uart_transmit('t');

		_delay_ms(1000);

	}
	return 0;
}

void startConversion()
{
	ADCSRA |= (1 << ADSC);

}


void ADCsetup()
{

	ADMUX = (1 << REFS0) | (1 << MUX0) | (1 << MUX2);
	ADCSRA = (1 << ADEN) | (1 << ADIE) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);

	DIDR0 = (1 << ADC5D); //disables the digital input buffer

	startConversion();
}

unsigned char GetAdc_value(void)
{
	ADCSRA |= (1<<ADSC);	// Start A to D conversion
	while (ADCSRA & 1<<ADSC);	// Wait for conversion to complete
	return (ADCH);
}

void uart_transmit(unsigned char value)
{
	// wait for empty transmit buffer
	while ( ! ( UCSR1A & ( 1 << UDRE1 ) ) );
	// put data into buffer, sends data
	UDR1 = value;
	PORTE ^= LED_SET;


}


void uart_init(void)
{

	UBRR1H = (unsigned char) (BRC >> 8);
	UBRR1L = BRC;

	//enable recieve and transmit
	UCSR1B = ( 1 << TXEN1 )| (1<<RXEN1);

	//set frame format
	UCSR1C = ( 1 << USBS1 ) | ( 3 << UCSZ10 );

}
