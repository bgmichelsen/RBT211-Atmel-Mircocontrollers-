/*
 * Michelsen_Assignment2-1.3.cpp
 *
 * Created: 9/10/2018 11:21:25 AM
 * Author : Brandon Michelsen
 */ 

#define F_CPU	1000000UL // 1 MHz
#define LED		0b00000001 // LED pin for this example (PB0)
#include <avr/io.h>
#include <util/delay.h>

// Function prototypes
void pwm_write(uint8_t pin_num, int duty_cycle); // Function to output PWM

int main(void)
{
	// Set DDRB Pin 0 to output
	DDRB |= LED;
	
    // Run infinitely
    while (1) 
    {
		// Increase the brightness of the LED
		for (int i = 0; i < 256; i++)
		{
			pwm_write(LED, i);
			_delay_ms(10);
		}
		
		// Decrease the brightness of the LED
		for (int i = 255; i > 0; i--)
		{
			pwm_write(LED, i);
			_delay_ms(10);
		}
    }
	
	return 0;
}

// Function definitions
void pwm_write(uint8_t pin_num, int duty_cycle)
{
	// Check what pin is being used for PWM
	if (pin_num == 0b00000001)
		TCCR0A = 0x81; // Set fast PWM for OC0A
	else if (pin_num == 0b00000010)
		TCCR0A = 0x21; // Set fast PWM for OC0B
	TCCR0B = 0x01; // 4 kHz signal
	
	// Set the duty cycle for the signal
	if (pin_num == 0b00000001)
		OCR0A = duty_cycle;
	else if (pin_num == 0b00000010) 
		OCR0B = duty_cycle;
}