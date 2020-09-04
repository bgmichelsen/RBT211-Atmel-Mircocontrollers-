/*
 * main.cpp
 *
 * Created: 10/10/2018 7:29:57 PM
 *  Author: Brandon Michelsen
 */ 

#define F_CPU	16000000UL // 16 MHz clock

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <m328/serial/USART.h>

// Function prototypes
int parse_int(Serial& S, char delimiter); // Function to parse an integer from a character array
void rgb_write(int r, int g, int b); // Function to write to an RGB LED

int main (void)
{
	// Setup inputs and outputs
	DDRD |= ((1<<DDD3) | (1<<DDD5) | (1<<DDD6));
	
	// Initialize serial
	Serial serial_com;
	serial_com.init(BAUD9600);
	
	// Declare local variables
	int r_val = 0; // Value for red
	int g_val = 0; // Value for green
	int b_val = 0; // Value for blue
	
	char buf1[8];
	char buf2[8];
	char buf3[8];
	
	while (1)
	{
		// Read the values
		r_val = parse_int(serial_com, ','); // Red value
		g_val = parse_int(serial_com, ','); // Green value
		b_val = parse_int(serial_com, ','); // Blue value
		
		// Write to the RGB LED
		rgb_write(r_val, g_val, b_val);
		
		// Convert the values to ASCII
		itoa(r_val, buf1, 10);
		itoa(g_val, buf2, 10);
		itoa(b_val, buf3, 10);
		
		// Print the values
		serial_com.print("\nR Value\t\tG Value\t\tB Value\n");
		serial_com.print("-------------------------------------\n");
		serial_com.print(buf1);
		serial_com.print("\t\t");
		serial_com.print(buf2);
		serial_com.print("\t\t");
		serial_com.print(buf3);
		serial_com.transmit('\n');
	}
}

// Function definitions
int parse_int(Serial& S, char delimiter)
{
	// Declare local variables
	char read_in = 'x'; // Variable for reading in serial data
	char buffer[20]; // Buffer for read values
	int i = 0; // Index value
	
	// Read in the character to the delimiter
	do
	{
		read_in = S.receive();
		buffer[i] = read_in;
		i++;
	} while (read_in != delimiter && read_in != '\n');
	
	return atoi(buffer);
}

void rgb_write(int r, int g, int b)
{
	/* Setup Timer 0 */
	TCCR0A = ((1<<COM0A1) | (1<<COM0B1) | (1<<WGM01) | (1<<WGM00)); // Set OC0A and OC0B for Fast PWM
	TCCR0B = (1<<CS00); // Clk/1, no prescaling
	OCR0A = r; // Set PWM duty cycle
	OCR0B = g;
	
	/* Setup Timer 2 */
	TCCR2A = ((1<<COM2B1) | (1<<WGM21) | (1<<WGM20)); // Set OC2B for Fast PWM
	TCCR2B = (1<<CS20); // Clk/1, no prescaling
	OCR2B = b;
}