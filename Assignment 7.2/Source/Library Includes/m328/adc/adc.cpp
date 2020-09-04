/*
 * adc.cpp
 *
 * Created: 10/15/2018 3:55:38 PM
 * Author : Brandon Michelsen
 */ 

#include <avr/io.h>

#include "adc.h"

// Function definitions
int adc_read_10(int channel)
{
	// Set digital inputs to be off on PORTC
	DIDR0 = 0x3F;
	
	// Read the proper channel
	if (channel == 0) ADMUX = 0x00; // Channel 0
	if (channel == 1) ADMUX = 0x01; // Channel 1
	if (channel == 2) ADMUX = 0x02; // Channel 2
	if (channel == 3) ADMUX = 0x03; // Channel 3
	if (channel == 4) ADMUX = 0x04; // Channel 4
	if (channel == 5) ADMUX = 0x05; // Channel 5
	
	// Read the analog value
	ADCSRA = ((1<<ADEN) | (1<<ADSC) | (1<<ADATE));
	
	return (ADCL + (256*ADCH));
}

int adc_read_8(int channel)
{
	// Set digital inputs to be off
	DIDR0 = 0x3F;
	
	// Set ADLAR bit for 8-bit operation
	ADMUX = (1<<ADLAR);
	
	// Set the proper channel
	if (channel == 0) ADMUX |= 0x00; // Channel 0
	if (channel == 1) ADMUX |= 0x01; // Channel 1
	if (channel == 2) ADMUX |= 0x02; // Channel 2
	if (channel == 3) ADMUX |= 0x03; // Channel 3
	if (channel == 4) ADMUX |= 0x04; // Channel 4
	if (channel == 5) ADMUX |= 0x05; // Channel 5
	
	// Read the analog value
	ADCSRA = ((1<<ADEN) | (1<<ADSC) | (1<<ADATE));
	
	// Return the read value
	return ADCH;
}
