/*
 * Michelsen_Assignment3-2.cpp
 *
 * Created: 9/19/2018 1:28:55 PM
 * Author : brand
 */ 

#define F_CPU	16000000UL // 16 MHz clock

#include <avr/io.h>
#include <util/delay.h>

// Function prototypes
void rgb_write(uint8_t r, uint8_t g, uint8_t b); // Function to write a color value to the RGB LED
void distance_map(uint8_t dist); // Function to map a distance value to an RGB value
uint8_t adc_read_8(int channel); // Function to read an analog value from the ADC
uint8_t distance_measurement(uint8_t val); // Function to linearize the analog value to a distance measurement
uint8_t red_map(uint8_t d);
uint8_t green_map(uint8_t d);
uint8_t blue_map(uint8_t d);

int main(void)
{
	// Setup inputs and outputs
	DDRD |= ((1<<DDD3) | (1<<DDD5) | (1<<DDD6));
	
	// Declare local variables
	uint8_t analog_val = 0; // Variable for initial analog read
	uint8_t distance_val = 0; // Variable for measured distance
	
    // Run infinitely
    while (1) 
    {
		analog_val = adc_read_8(0); // Read the analog value on channel 0
		distance_val = distance_measurement(analog_val); // Convert to linear distance
		
		// Light the LED
		distance_map(distance_val);
    }
}

// Function definitions
void rgb_write(uint8_t r, uint8_t g, uint8_t b)
{
	/* Set Timer 0 PWM */
	TCCR0A = ((1<<COM0A1) | (1<<COM0B1) | (1<<WGM00) | (1<<WGM01)); // Set fast PWM for OC0A and OC0B
	TCCR0B = (1<<CS00); // 4 kHz signal (no pre-scaler)
	OCR0A = g; 
	OCR0B = r;
	
	/* Set Timer 2 PWM */
	TCCR2A = ((1<<COM2B1) | (1<<WGM20) | (1<<WGM21)); // Fast PWM for OC2B
	TCCR2B = (1<<CS20); // 4 kHz signal (no pre-scaler)
	OCR2B = b;
}

uint8_t adc_read_8(int channel)
{
	DIDR0 = 0x3F; // Turn off inputs on Port C
	
	// Set ADC to 8-bit mode
	ADMUX = (1<<ADLAR);
	
	// Select the proper channel
	if (channel == 0) ADMUX |= 0x00; // Channel 0
	if (channel == 1) ADMUX |= 0x01; // Channel 1
	if (channel == 2) ADMUX |= 0x02; // Channel 2
	if (channel == 3) ADMUX |= 0x03; // Channel 3
	if (channel == 4) ADMUX |= 0x04; // Channel 4
	if (channel == 5) ADMUX |= 0x05; // Channel 5
	
	// Set ADC to begin reading
	ADCSRA = ((1<<ADEN) | (1<<ADSC) | (1<<ADATE));
	
	_delay_ms(1); // Delay 1 ms
	
	// Read the analog value
	return ADCH;
}

uint8_t distance_measurement(uint8_t val)
{
	// Linearize the analog value to output
	return (6787/(val + 5) - 1);
}

void distance_map(uint8_t dist)
{
	rgb_write(red_map(dist), green_map(dist), blue_map(dist));
}

uint8_t red_map(uint8_t d)
{
	// Value to return
	uint8_t val = 0;
	
	// Check to make sure we are in range
	if ((d >= 10 && d <= 40) || (d >= 60 && d <= 80))
		val = (17*((d - 10)*(d - 80)))/100;
	return val;
}

uint8_t green_map(uint8_t d)
{
	// Value to return
	uint8_t val = 0;
	
	// Check to make sure we are within range
	if (d >= 10 && d <= 60)
		val = (17*((d - 10)*(d - 60)))/(-40);
	return val;
}

uint8_t blue_map(uint8_t d)
{
	// Value to return
	uint8_t val = 0;
	
	// Check to make sure we are within range
	if (d >= 40 && d <= 80)
		val = (51*((d - 40)*(d - 80)))/(-80);
	return val;
}