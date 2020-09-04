/*
 * Michelsen_Assignment7-2.cpp
 *
 * Created: 10/17/2018 7:14:40 PM
 * Author : Brandon Michelsen
 */ 

#define F_CPU	16000000UL // 16 MHz clock

#include <avr/io.h>
#include <util/delay.h>
#include <m328/adc/adc.h>
#include <m328/shift/shift_register.h>

// Function prototypes
int _map(int x, int in_min, int in_max, int out_min, int out_max); // Function to map one number range to another
void delay(int time); // Function for a variable delay

int main(void)
{
	// Setup inputs and outputs
	DDRD |= ((1<<DATA) | (1<<LATCH) | (1<<SH_CLK));
	
	// Declare local variables
	int delay_time = 0; // Variable for tracking the speed of light rider traversal 
	int index = 0; // Variable for tracking the index of the light rider LEDs
	
	// Array for storing the light rider LED values (positions)
	uint8_t light_rider[14] = {
		1, 2, 4, 8, 16, 32, 64, 128, 64, 32, 16, 8, 4, 2
	};
	
    // Run infinitely
    while (1) 
    {
		// Read the analog signal
		delay_time = adc_read_10(0);
		
		// Map the value to be in the range of millsecond delays
		delay_time = _map(delay_time, 0, 1023, 20, 5);
		
		// Shift out to the register to run the light rider sequence
		shift_out(MSB_FIRST, light_rider[index]);
		
		// Adjust the delay time
		delay(delay_time);
		
		// Increment the index unless the index is above 13
		if (index >= 13)
			index = 0;
		else
			index++;
    }
}

// Function definitions
int _map(int x, int in_min, int in_max, int out_min, int out_max)
{
	// Map the data to an output range
	return (x - in_min)*(((float)(out_max - out_min))/(in_max - in_min)) + out_min;
}

void delay(int time)
{
	// Delay for 1 ms up until the given value
	for (int i = 0; i < time; i++)
		_delay_ms(1);
}