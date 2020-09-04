/*
 * Michelsen_Assignment2-1.6.cpp
 *
 * Created: 9/10/2018 3:43:22 PM
 * Author : brand
 */ 
#define F_CPU	1000000UL // 1 MHz

#define SWITCH	(1<<PB0) // Byte for the switch pin (PB0)
#define BLED	(1<<PB1) // Byte for blue pin (PB1)
#define RLED	(1<<PD5) // Byte for red pin (PD5)
#define GLED	(1<<PD6) // Byte for green pin (PD6)

#include <avr/io.h>
#include <util/delay.h>

// Function prototypes
void rgb_out(uint8_t r, uint8_t g, uint8_t b); // Function for controlling the RGB LED
void rgb_mode(int count); // Function for selecting a mode for the RGB LED
bool debounce(uint8_t pin, bool last); // Function for debouncing a switch

int main(void)
{
	// Set inputs and outputs
	DDRB |= BLED; // Set PB1 to be an output
	DDRB &= ~SWITCH; // Set PB0 to be an input
	
	DDRD |= (RLED | GLED); // Set PD5 and PD6 to be outputs

	// Declare local variables
	bool current_state = 0; // Variable for tracking the button state
	bool last_state = 0; // Variable for tracking the previous button state
	int mode_count = 0; // Variable for tracking button presses (used to change the mode)
	

    // Run infinitely
    while (1) 
    {
		// Read the current button state
		current_state = debounce(SWITCH, last_state);

		// Check if the button is being pressed
		if (current_state == 1 && last_state == 0)
			// Update the count
			mode_count++;
		// Store the current value as the new previous
		last_state = current_state;
		
		// Make sure the mode count does not go over 7
		if (mode_count > 7)
			mode_count = 0;
		
		// Write the value to the RGB LED
		rgb_mode(mode_count);
	}
	
	return 0;
}

// Function definitions
void rgb_out(uint8_t r, uint8_t g, uint8_t b)
{
	/* Set PD5 and PD6 PWM Output */
	TCCR0A |= 0xA3; // Set both OC0A and OC0B
	TCCR0B |= 0x05; // Rate = CLK/1024
	OCR0A = g; // Run OCR0A at r
	OCR0B = r; // Run OCR0B at g
	
	/* Set PB1 PWM Output */
	ICR1 = 0x00FF; // Set TOP to 8 bits
	TCCR1A |= 0xA1; // Set up fast PWM
	TCCR1B |= 0x0D; // Rate = CLK/1024
	OCR1A = b; // Run OCR1A at b
}

bool debounce(uint8_t pin, bool last)
{
	// Read the button state
	bool current = (PINB & SWITCH);
	
	// Check if the state has changed
	if (current != last)
	{
		// Wait 5 ms
		_delay_ms(5);
		
		// Re-read the button state
		current = (PINB & SWITCH);	
	}
	
	// Return result
	return current;
}

void rgb_mode(int count)
{
	// Check the count for the mode
	if (count == 1)
		// Mode 1, Red output
		rgb_out(255, 0, 0);
	else if (count == 2)
		// Mode 2, Green output
		rgb_out(0, 255, 0);
	else if (count == 3)
		// Mode 3, Blue output
		rgb_out(0, 0, 255);
	else if (count == 4)
		// Mode 4, Purple output
		rgb_out(127, 0, 127);
	else if (count == 5)
		// Mode 5, Teal output
		rgb_out(0, 127, 127);
	else if (count == 6)
		// Mode 6, Orange output
		rgb_out(127, 127, 0);
	else if (count == 7)
		// Mode 7, white output
		rgb_out(85, 85, 85);
	else
		// Otherwise, turn off
		rgb_out(0, 0, 0);
}