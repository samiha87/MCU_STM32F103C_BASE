/*
 * GPIO.h
 *
 *  Created on: Jul 29, 2019
 *      Author: samih
 */

#ifndef __GPIO_H_
#define __GPIO_H_

#include <stdio.h>

class GPIO {
public:

	// Constructor
	GPIO();
	virtual ~GPIO();

	enum enumPin {
		PIN_PA0,
		PIN_PA1,
		PIN_PA2,
		PIN_PA3,
		PIN_PA4,
		PIN_PA5,
		PIN_PA6,
		PIN_PA7,
		PIN_PA8,
		PIN_PA9,
		PIN_PA10,
		PIN_PA11,
		PIN_PA12,
		PIN_PA13,
		PIN_PA14,
		PIN_PA15,
		PIN_PB0,
		PIN_PB1,
		PIN_PB2,
		PIN_PB3,
		PIN_PB4,
		PIN_PB5,
		PIN_PB6,
		PIN_PB7,
		PIN_PB8,
		PIN_PB9,
		PIN_PB10,
		PIN_PB11,
		PIN_PB12,
		PIN_PB13,
		PIN_PB14,
		PIN_PB15
	};

	enum enumType {
		// Output types
		PIN_OUTPUT,
		PIN_INPUT,
		// Output state
		PIN_HIGH,
		PIN_LOW
	};

	void initPin(enumPin ePin, enumType pinType);
	void setPin(enumPin ePin, enumType);
	enumType getPin(enumPin ePin);

private:
	// returns corresponding pin from STM libraries
	uint16_t getCorrectPin(enumPin pin);
	// Configure pins as input or output
	void initGPIOA (uint16_t pin_x, int type);
	void initGPIOB (uint16_t pin_x, int type);

	// Set pin high or low
	void writeGPIOA(uint16_t pin_x, int state);
	void writeGPIOB(uint16_t pin_x, int state);

	// Read pin status, return HIGH or LOW
	int readGPIOA(uint16_t pin_x);
	int readGPIOB(uint16_t pin_x);

};
#endif
