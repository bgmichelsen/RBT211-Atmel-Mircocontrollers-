/*
 * Michelsen_Assignment3-1.2.cpp
 *
 * Created: 9/17/2018 3:37:04 PM
 * Author : Brandon Michelsen
 */ 

#define F_CPU	16000000UL // 16 MHz

#define LOW_THRESHOLD	25 // Lower bound of light range
#define HIGH_THRESHOLD	45 // Upper bound of light range

#include <avr/io.h>
#include <util/delay.h>

// Function prototypes
void rgb_write(uint8_t r, uint8_t g, uint8_t b); // Function to write to an RGB LED
uint8_t adc_read_8(int channel); // Function to read an analog value

int main(void)
{
	// Set inputs and outputs
	DDRD |= ((1<<DDD3) | (1<<DDD5) | (1<<DDD6));
	
	// Declare local variables
	int temp_val = 0; // Variable for reading the photo-resistor
	
    // Run infinitely
    while (1) 
    {
		temp_val = adc_read_8(0); // Read in the value of the photo-resistor connected to channel 0
		
		// Display the values based on what it is at
		if (temp_val > HIGH_THRESHOLD)
			// Display a red color
			rgb_write(255, 0, 0);
		else if (temp_val < LOW_THRESHOLD)
			// Display a blue color
			rgb_write(0, 0, 255);
		else
			// Display a green color
			rgb_write(0, 255, 0);
    }
	
	return 0;
}

// Function definitions
void rgb_write(uint8_t r, uint8_t g, uint8_t b)
{
	/* Timer 0 PWM Setup */
	TCCR0A = ((1<<COM0A1) | (1<<COM0B1) | (1<<WGM00) | (1<<WGM01)); // Fast PWM for OC0A and OC0B
	TCCR0B = (1<<CS00); // 4kHz (no pre-scaler)
	OCR0A = g; // Write the PWM signal at the desired duty cycles
	OCR0B = r;
	
	/* Timer 2 PWM Setup */
	TCCR2A = ((1<<COM2B1) | (1<<WGM20) | (1<<WGM21)); // Fast PWM for OC2B
	TCCR2B = (1<<CS20); // 4kHz (no pre-scaler)
	OCR2B = b; // Write t PWM signal at the desired duty cycle
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