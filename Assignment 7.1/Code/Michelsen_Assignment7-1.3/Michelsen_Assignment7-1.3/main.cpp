/*
 * Michelsen_Assignment7-1.3.cpp
 *
 * Created: 10/15/2018 3:46:10 PM
 * Author : Brandon Michelsen
 */ 

#define F_CPU	16000000UL // 16 MHz clock

#include <avr/io.h>
#include <util/delay.h>
#include <m328/shift/shift_register.h>
#include <m328/adc/adc.h>

// Function prototypes
int _map(int x, int in_min, int in_max, int out_min, int out_max); // Function to map one number range to another

int main(void)
{
	// Setup inputs and outputs
	DDRD |= ((1<<DATA) | (1<<SH_CLK) | (1<<LATCH));
	
	// Declare local variables
	int analog_val = 0;
	int mapped_val = 0;
	
	uint8_t values[9] = {
		255, 127, 63, 31, 15, 7, 3, 1, 0
	};
	
    // Run infinitely
    while (1) 
    {
		// Read the analog value
		analog_val = adc_read_10(0);
		
		// Map it to the index for our values array
		mapped_val = _map(analog_val, 0, 1023, 0, 8);
		
		// Write the corresponding value to the shift register
		shift_out(MSB_FIRST ,values[mapped_val]);
    }
}

// Function definitions
int _map(int x, int in_min, int in_max, int out_min, int out_max)
{
	// Map the data to an output range
	return (x - in_min)*(((float)(out_max - out_min))/(in_max - in_min)) + out_min;
}