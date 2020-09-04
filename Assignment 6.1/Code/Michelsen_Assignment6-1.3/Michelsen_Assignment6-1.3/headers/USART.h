/*
 * USART.h
 *
 * Created: 9/28/2018 2:30:06 PM
 *  Author: brand
 */ 


#ifndef USART_H_
#define USART_H_

// Class for using USART0 on ATmega328
class Serial
{
	// PD1 is TX, PD0 is RX
	// UBRR0 = baudrate = frequency/(8*Baud)-1
	// Baud rate table (based on above equation)
	// 9600		12
	// 4800		25
	// 2400		51
	// 1200		103
private:
	// No private elements
public:
	// Baud rate definitions
	#define BAUD9600	12
	#define BAUD4800	25
	#define BAUD2400	51
	#define BAUD1200	103
	
	// Method prototypes
	void init(unsigned char baud_rate);
	void transmit(unsigned char val);
	void print(char* val);
	char receive();
};



#endif /* USART_H_ */