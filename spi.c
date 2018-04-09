#include <avr/io.h>

void
spi_init(void)
{
	//PRR0 |= (0<<PRSPI);
	/* Port setup */
	/* Set PB0 (nRF reset), PB2 (RDYN), PB3 (MOSI) and PB5 (SCK) as output */
	DDRB = (1 << PB0) | (1 << PB2)  | (1 << PB2) | (1 << PB3) | (1 << PB5);
	PORTB = 0xff;

    /* master, SPI mode 0 */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0) | (1 << DORD);
}

/**
 * SPI data transmission.
 * Transmit a byte of data over SPI, wait until the transfer is completed
 * and return the read back value coming from the SPI device - if any.
 *
 * @param data Data to transmit
 * @return Read back data from the device
 */
uint8_t
spi_transmit(uint8_t data)
{
    SPDR = data;

    while (!(SPSR & (1 << SPIF))) {
        /* wait */
    }

    return SPDR;
}


