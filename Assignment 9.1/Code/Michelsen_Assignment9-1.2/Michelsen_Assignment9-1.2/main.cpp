/*
 * Michelsen_Assignment9-1.2.cpp
 *
 * Created: 10/31/2018 8:20:06 PM
 * Author : Brandon Michelsen
 */ 

#define F_CPU	16000000UL // 16 MHz clock

// Define the slave-select pins
#define SS1		PORTB1
#define SS2		PORTB2

// Define the byte commands needed to send to the digipots
#define REG0_WRITE		0b00010001 // Write to pot 0
#define REG1_WRITE		0b00010010 // Write to pot 1
#define TWO_POT_WRITE	0b00010011 // Write to both pots

#include <avr/io.h>
#include <util/delay.h>
#include <m328/spi/spi.h>
#include <m328/tone/tone.h>
#include <m328/tone/Notes.h>

// Function prototypes
void digipot_set_LED(uint8_t SS, uint8_t reg, uint8_t val); // Function to write values to the digital potentiometer

int main(void)
{
	// Setup chip select pins
	DDRB |= ((1<<SS1) | (1<<SS2));
	DDRD |= (1<<DDD6);
	
	// Set slave select pins to high
	PORTB |= ((1<<SS1) | (1<<SS2));
	
	// Initiate SPI
	init_SPI();
	
	// Declare local variables
	uint8_t j = 0; // Loop variable
	int freq = 100; // Frequency variable
	
	// Run infinitely
	while (1)
	{
		// Play the frequency
		tone(freq, 0);
		
		// Increase light
		for (j = 0; j < 255; j++)
		{
			digipot_set_LED(SS1, REG0_WRITE, j);
			digipot_set_LED(SS1, REG1_WRITE, j);
			digipot_set_LED(SS2, REG0_WRITE, j);
			digipot_set_LED(SS2, REG1_WRITE, j);
			_delay_ms(10);
		}
		// Decrease light
		for (j = 255; j > 0; j--)
		{
			digipot_set_LED(SS1, REG0_WRITE, j);
			digipot_set_LED(SS1, REG1_WRITE, j);
			digipot_set_LED(SS2, REG0_WRITE, j);
			digipot_set_LED(SS2, REG1_WRITE, j);
			_delay_ms(10);
		}
		
		// Increase the frequency
		freq += 100;
		if (freq > 2000) freq = 100;
	}
}

// Function definitions
void digipot_set_LED(uint8_t SS, uint8_t reg, uint8_t val)
{
	// Set the SS pin low
	PORTB &= ~(1<<SS);
	
	// Transfer the command byte
	trade_byte_SPI(reg);
	
	// Transfer the value
	trade_byte_SPI(val);
	
	// Set the SS pin high again
	PORTB |= (1<<SS);
}

