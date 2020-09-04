/*
 * Michelsen_Assignment2-1.2.cpp
 *
 * Created: 9/10/2018 10:54:13 AM
 * Author : Brandon Michelsen
 */ 

#define F_CPU 1000000UL // Frequency: 1 MHz
#include <avr/io.h>
#include <util/delay.h>

// Function prototypes
void my_delay_ms(int milliseconds); // Function for variable delays

int main(void)
{
	// Setup DDRB Pin 3 as an output
	DDRB |= (1<<DDB3);
	
    // Run infinitely
    while (1) 
    {
		// Run a for-loop that blink the LED with a delay dependent on i
		for (int i = 100; i < 1000; i += 100)
		{
			PORTB |= (1<<PB3);
			my_delay_ms(i);
			PORTB &= ~(1<<PB3);
			my_delay_ms(i);
		}
    }
	
	return 0;
}

// Function definitions
void my_delay_ms(int milliseconds)
{
	// Run until the milliseconds are 0
	while (milliseconds != 0)
	{
		_delay_ms(1);
		milliseconds--;
	}
}