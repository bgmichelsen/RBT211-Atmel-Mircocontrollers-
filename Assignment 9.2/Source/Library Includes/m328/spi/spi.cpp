/*
 * spi.cpp
 *
 * Created: 10/30/2018 6:17:45 PM
 * Author : Brandon Michelsen
 */ 

#include <avr/io.h>
#include "spi.h"

// Function Definitions
void init_SPI(void)
{
	// Setup SPI port
	SPI_DDR |= ((1<<SPI_MOSI) | (1<<SPI_MISO) | (1<<SPI_SCK));
	
	// Setup SPI
	SPCR |= ((1<<SPR1) | (1<<MSTR) | (1<<SPE));
}

void trade_byte_SPI(uint8_t byte)
{
	// Load a byte into the transfer register
	SPDR = byte;
	
	// Loop until the bit is set
	while (!(SPSR & (1<<SPIF)));
}