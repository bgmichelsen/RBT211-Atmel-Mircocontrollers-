/*
 * Michelsen_Assignment2-1.4.cpp
 *
 * Created: 9/10/2018 1:11:39 PM
 * Author : Brandon Michelsen
 */ 

#define F_CPU 1000000UL // 1 MHz

#define LED		0b00001000 // LED pin
#define SWITCH	0b00010000 // Switch pin

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	// Setup inputs and outputs
	DDRB |= LED; // Set LED pin as output
	DDRB &= ~SWITCH; // Set switch pin as input
	
    // Run infinitely
    while (1) 
    {
		// Read an input
		if ((PINB & SWITCH) == 0)
		{
			// If the switch is low, turn off LED
			PORTB &= ~LED;
		}
		else {
			// Otherwise, turn on the LED
			PORTB |= LED;
		}
    }
	
	return 0;
}

