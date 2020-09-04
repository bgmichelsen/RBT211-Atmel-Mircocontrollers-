/*
 * Michelsen_Assignment2-1.5.cpp
 *
 * Created: 9/10/2018 1:44:09 PM
 * Author : Brandon Michelsen
 */ 
#define F_CPU	1000000UL // 1 MHz

#define LED		0b00001000 // LED pin (PB3)
#define SWITCH	0b00010000 // Switch pin (PB4)

#include <avr/io.h>
#include <util/delay.h>

// Function prototypes
bool debounce(uint8_t pin_num, bool prev_state); // Function for debouncing a switch
void digital_write(uint8_t pin_num, bool value); // Function for writing a digital value to a pin

int main(void)
{
	// Set inputs and outputs
	DDRB |= LED; // Set LED as an output
	DDRB &= ~SWITCH; // Set SWITCH as an input
	
	// Declare local variables
	bool current_state = 0; // Variable for reading the button's current state
	bool last_state = 0; // Variable for reading the previous state of the button
	bool led_on = 0; // Variable for turning on/off the LED
	
    // Run infinitely
    while (1) 
    {
		// Read the current button state
		current_state = debounce(LED, last_state);
		
		// Check for the button being pressed
		if (current_state == 1 && last_state == 0)
			led_on = !led_on; // Toggle the LED
		
		// Reset the previous state
		last_state = current_state;
		
		// Write the LED state
		digital_write(LED, led_on);
    }
	
	return 0;
}

// Function definitions
bool debounce(uint8_t pin_num, bool prev_state)
{
	// Read the button state
	bool current = (PINB & SWITCH);
	
	// Check if it has changed state
	if (current != prev_state)
	{
		// Delay 5 ms
		_delay_ms(5);
		
		// Re-read the button
		current = (PINB & SWITCH);
	}
	
	// Return the value
	return current;
}

void digital_write(uint8_t pin_num, bool value)
{
	// Check the value
	if (value == 1)
		// If the value is one, set the pin to high
		PORTB |= pin_num;
	else
		// Otherwise, set the pin to low
		PORTB &= ~pin_num;
}

