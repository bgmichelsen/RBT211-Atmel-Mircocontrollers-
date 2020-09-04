/*
 * Michelsen_Assignment3-1.1.cpp
 *
 * Created: 9/17/2018 11:38:21 AM
 * Author : Brandon Michelsen
 */ 

#define F_CPU 16000000UL // 1 MHz

#include <avr/io.h>
#include <util/delay.h>

// Function prototypes
uint8_t adc_read_8(int channel); // Function to read an adc value
void pwm_write(uint8_t duty_cycle); // Function for writing a PWM signal

int main(void)
{
	// Setup inputs and outputs
	DDRD |= (1<<DDD6);
	
	// Run infinitely
	while (1)
	{
		// Read analog signal
		uint8_t analog_val = adc_read_8(0);
		
		// Write it to PWM
		pwm_write(analog_val);
	}
	
	return 0;
}

// Function definitions
void pwm_write(uint8_t duty_cycle)
{
	TCCR0A = ((1<<COM0A1) | (1<<WGM00) | (1<<WGM01)); // Fast PWM for OC0A
	TCCR0B = (1<<CS00); // Rate is 4 kHz (no pre-scaler)
	OCR0A = duty_cycle;
}

uint8_t adc_read_8(int channel)
{
	DIDR0 = 0x3F; // Turn off digital inputs on PORTA
	
	// Set ADLAR bit so that it is in 8-bit mode
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
	_delay_ms(1);
	
	// Return the read value
	return ADCH;
}