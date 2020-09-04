/*
 * spi.h
 *
 * Created: 10/30/2018 6:18:34 PM
 *  Author: Brandon Michelsen
 */ 


#ifndef SPI_H_
#define SPI_H_

// Definitions
#define SPI_DDR		DDRB
#define SPI_MOSI	PORTB3
#define SPI_MISO	PORTB4
#define SPI_SCK		PORTB5

// Function Prototypes
void init_SPI(void);
void trade_byte_SPI(uint8_t byte);

#endif /* SPI_H_ */