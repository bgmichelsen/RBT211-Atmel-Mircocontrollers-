/*
 * Michelsen_Assignment7-1.1.cpp
 *
 * Created: 10/15/2018 12:34:40 PM
 * Author : Brandon Michelsen
 */ 

#define F_CPU	16000000UL // 16 MHz clock

#define R_PORT	PORTD
#define SH_CLK	PD4
#define LATCH	PD3
#define DATA	PD2

#define DATA_HIGH()		(R_PORT |= (1<<DATA))
#define DATA_LOW()		(R_PORT &= ~(1<<DATA))
#define SH_CLK_HIGH()	(R_PORT |= (1<<SH_CLK))
#define SH_CLK_LOW()	(R_PORT &= ~(1<<SH_CLK))
#define LATCH_HIGH()	(R_PORT |= (1<<LATCH))
#define LATCH_LOW()		(R_PORT &= ~(1<<LATCH))			

#define MSB_FIRST	1
#define LSB_FIRST	0	

#include <avr/io.h>
#include <util/delay.h>

// Function prototypes
void shift_out(uint8_t bit_dir, uint8_t data);

int main(void)
{
	// Setup inputs and outputs
	DDRD |= ((1<<DATA) | (1<<LATCH) | (1<<SH_CLK));
	
	// Output a single pattern to the shift register
	shift_out(MSB_FIRST, 0b10101010);
	
    // Run infinitely
    while (1) 
    {
    }
}

// Function definitions
void shift_out(uint8_t bit_dir, uint8_t data)
{
	// Set the latch low (to keep data exchange from changing current output)
	LATCH_LOW();
	
	// Set everything else low for proper data transmission
	DATA_LOW();
	SH_CLK_LOW();
	
	// Serially send the data
	for (uint8_t i = 1; i <= 8; i++)
	{
		// If the bit direction is MSB first, send that
		if (bit_dir == MSB_FIRST)
		{
			if (data & 0b10000000)
				// If the current bit is set, send a 1
				DATA_HIGH();
			else
				// Otherwise, send a 0
				DATA_LOW();
		
			// Get the next bit in the data
			data = (data << 1);
		}
		else
		{
			// Otherwise, send the LSB first
			if (data & 0b00000001)
				// If the current bit is set, send a 1
				DATA_HIGH();
			else
				// Otherwise, send a 0
				DATA_LOW();
			
			// Get the next bit in the data
			data = (data >> 1);
		}
		
		// Pulse the clock
		SH_CLK_HIGH();
		SH_CLK_LOW();
	}
	
	// Reset the latch to high to complete the data transfer
	LATCH_HIGH();
}