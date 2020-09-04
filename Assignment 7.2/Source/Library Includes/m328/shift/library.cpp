/*
 * shift_register.cpp
 *
 * Created: 10/15/2018 3:01:37 PM
 * Author : Brandon Michelsen
 */ 

#include <avr/io.h>
#include "shift_register.h"

// Function definitions
void shift_init()
{
	// Initialize the port for using the shift register
	R_PORT |= ((1<<DATA) | (1<<SH_CLK) | (1<<LATCH));
}

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