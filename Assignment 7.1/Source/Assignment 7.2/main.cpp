/*
 * Michelsen_Assignment7-1.2.cpp
 *
 * Created: 10/15/2018 3:11:19 PM
 * Author : Brandon Michelsen
 */ 

#define F_CPU	16000000UL // 16 MHz clock

#include <avr/io.h>
#include <util/delay.h>
#include <m328/shift/shift_register.h>

int main(void)
{
	// Initiate the shift register
	DDRD |= ((1<<DATA) | (1<<LATCH) | (1<<SH_CLK));
		
    // Run infinitely
    while (1) 
    {
		// Cycle through and animate the light rider effect
		for (uint8_t i = 0; i < 8; i++)
		{
			shift_out(MSB_FIRST, (1<<i));
			_delay_ms(50);
		}
		for (uint8_t j = 7; j > 0; j--)
		{
			shift_out(MSB_FIRST, (1<<j));
			_delay_ms(50);
		}
    }
}

