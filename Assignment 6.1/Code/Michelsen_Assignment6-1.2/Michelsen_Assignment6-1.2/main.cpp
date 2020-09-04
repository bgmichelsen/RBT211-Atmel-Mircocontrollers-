/*
 * Michelsen_Assignment6-1.2.cpp
 *
 * Created: 10/10/2018 4:18:37 PM
 * Author : brand
 */ 

#define F_CPU	16000000UL // 16 MHz clock

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "headers/USART.h"

// Function prototypes
int analog_read_10(int channel); // Function to read a value from the ADC
int _map(int x, int in_min, int in_max, int out_min, int out_max); // Function to map a number range to another number range

int main(void)
{
	// Inititialize serial communication
	Serial serial_com;
	serial_com.init(BAUD9600);
	
	// Declare local variables
	int analog_val = 0;
	int mapped_val = 0;
	char buf1[8];
	char buf2[8];
	
	// Run infinitely
	while (1)
	{
		// Print formetting
		serial_com.print("\nAnalog Pin\tRaw Value\tPercentage\n");
		serial_com.print("--------------------------------------------\n");
		
		// Print the analog values
		for (int i = 0; i < 10; i++)
		{
			// Read the analog signal
			analog_val = analog_read_10(0);
			
			// Map the value to a percentage
			mapped_val = _map(analog_val, 0, 1023, 0, 100);
			
			// Convert the numbers to character arrays
			itoa(analog_val, buf1, 10);
			itoa(mapped_val, buf2, 10);
			
			// Print out the values
			serial_com.print("A0\t\t");
			serial_com.print(buf1);
			serial_com.print("\t\t");
			serial_com.print(buf2);
			serial_com.print("%\n");
			
			_delay_ms(500); // Delay 1 second			
		}
	}
}

// Function definitions
int analog_read_10(int channel)
{
	// Set digital I/O to be off on Port C
	DIDR0 = 0x3F;
	
	// Get the proper ADC channel
	if (channel == 0) ADMUX = 0x00; // Channel 0
	if (channel == 1) ADMUX = 0x01; // Channel 1
	if (channel == 2) ADMUX = 0x02; // Channel 2
	if (channel == 3) ADMUX = 0x03; // Channel 3
	if (channel == 4) ADMUX = 0x04; // Channel 4
	if (channel == 5) ADMUX = 0x05; // Channel 5
	
	// Read the ADC signal
	ADCSRA = ((1<<ADEN) | (1<<ADSC) | (1<<ADATE));
	int val = ADCL;
	val += 256*ADCH;
	return val;
}

int _map(int x, int in_min, int in_max, int out_min, int out_max)
{
	// Map the data to an output range
	return (x - in_min)*(((float)(out_max - out_min))/(in_max - in_min)) + out_min;
}

