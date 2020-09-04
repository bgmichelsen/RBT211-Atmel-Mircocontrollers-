/*
 * Michelsen_Assignment9-2.cpp
 *
 * Created: 11/2/2018 9:41:59 AM
 * Author : Brandon Michelsen
 */ 

#define F_CPU	16000000UL // 16 MHz clock

// Define pins used for chip-select
#define SS1		PORTB1
#define SS2		PORTB2

// Define SPI commands
#define REG0_WRITE		0b00010001
#define REG1_WRITE		0b00010010
#define TWO_REG_WRITE	0b00010011

#include <avr/io.h>
#include <util/delay.h>
#include <m328/adc/adc.h>
#include <m328/spi/spi.h>
#include <m328/tone/tone.h>

// Function prototypes
void digipot_write(uint8_t ss, uint8_t reg, uint8_t data);

int main(void)
{
	// Setup inputs and outputs
	DDRB |= ((1<<SS1) | (1<<SS2));
	DDRD |= (1<<DDD6);
	
	// Initiate SPI
	init_SPI();
	
	// Declare local variables
	uint8_t analog_val = 0; // Variable for reading ADC values
	
    // Run infinitely
    while (1) 
    {
		// Read the analog value
		analog_val = adc_read_8(0);
		
		// Play a tone on the speaker (100 Hz)
		tone(100, 0);
		
		// Write it out to the SPI potentiometers
		digipot_write(SS1, TWO_REG_WRITE, analog_val);
		digipot_write(SS2, TWO_REG_WRITE, analog_val);
    }
}

// Function definitions
void digipot_write(uint8_t ss, uint8_t reg, uint8_t data)
{
	// Set the chip-select low
	PORTB &= ~(1<<ss);
	
	// Write the register command to the SPI device
	trade_byte_SPI(reg);
	
	// Write the data to the SPI device
	trade_byte_SPI(data);
	
	// Set the chip-select high again
	PORTB |= (1<<ss);
}