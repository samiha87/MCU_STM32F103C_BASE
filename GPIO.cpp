#include "GPIO.h"
#include "defines.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

GPIO_InitTypeDef GPIO_InitStruct_GPIO;

GPIO::GPIO() {

}
GPIO::~GPIO() {
	// TODO Auto-generated destructor stub
}

void GPIO::initPin(enumPin ePin, enumType pinType) {
	int state = 0;

	if(pinType == PIN_OUTPUT) state = OUTPUT;
	else if(pinType == PIN_INPUT) state = INPUT;
	else return;	// Not initialzed correctly

	if(ePin > PIN_PA15) {
		initGPIOB(getCorrectPin(ePin), state);
	} else {
		initGPIOA(getCorrectPin(ePin), state);
	}
}

void GPIO::setPin(enumPin ePin, enumType pinState) {
	int state = 0;

	if(pinState == PIN_HIGH) state = HIGH;
	else if(pinState == PIN_LOW) state = LOW;
	else return;	// Not initialzed correctly

	if(ePin > PIN_PA15) {
		writeGPIOB(getCorrectPin(ePin), state);
	} else {
		writeGPIOA(getCorrectPin(ePin), state);
	}
}

GPIO::enumType GPIO::getPin(enumPin ePin) {

	int state = 0;
	if(ePin > PIN_PA15) {
		state = readGPIOB(getCorrectPin(ePin));
	} else {
		state = readGPIOA(getCorrectPin(ePin));
	}
	if(!state) return PIN_HIGH;
	return PIN_LOW;
}

uint16_t GPIO::getCorrectPin(enumPin pin) {
	int choice = pin;

	// If enum is higher than PA15 we are dealing win portb pins.
	// PORT A and B pins are equal value.
	if(choice > PIN_PA15) choice = (pin - PIN_PA15);

	switch(choice) {
	case 0:
		return GPIO_Pin_0;
	case 1:
		return GPIO_Pin_1;
	case 2:
		return GPIO_Pin_2;
	case 3:
		return GPIO_Pin_3;
	case 4:
		return GPIO_Pin_4;
	case 5:
		return GPIO_Pin_5;
	case 6:
		return GPIO_Pin_6;
	case 7:
		return GPIO_Pin_7;
	case 8:
		return GPIO_Pin_8;
	case 9:
		return GPIO_Pin_9;
	case 10:
		return GPIO_Pin_10;
	case 11:
		return GPIO_Pin_11;
	case 12:
		return GPIO_Pin_12;
	case 13:
		return GPIO_Pin_13;
	case 14:
		return GPIO_Pin_14;
	case 15:
		return GPIO_Pin_15;
	default:
		return 0;
		break;
	}
	return 0;
}

void GPIO::initGPIOA (uint16_t pin_x, int type) {
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// Configure PA0 as push-pull output
	GPIO_InitStruct_GPIO.GPIO_Pin = pin_x;
	GPIO_InitStruct_GPIO.GPIO_Speed = GPIO_Speed_2MHz;
	if (type == OUTPUT) GPIO_InitStruct_GPIO.GPIO_Mode = GPIO_Mode_Out_PP;	// Set as output
	if (type == INPUT) GPIO_InitStruct_GPIO.GPIO_Mode = GPIO_Mode_IPU;	// Set as input
	
	GPIO_Init(GPIOA, &GPIO_InitStruct_GPIO);
}

void GPIO::initGPIOB (uint16_t pin_x, int type) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	// Configure PA0 as push-pull output
	GPIO_InitStruct_GPIO.GPIO_Pin = pin_x;
	GPIO_InitStruct_GPIO.GPIO_Speed = GPIO_Speed_2MHz;
	if (type == OUTPUT) GPIO_InitStruct_GPIO.GPIO_Mode = GPIO_Mode_Out_PP;	// Set as output
	if (type == INPUT) GPIO_InitStruct_GPIO.GPIO_Mode = GPIO_Mode_IPU;	// Set as input
	GPIO_Init(GPIOB, &GPIO_InitStruct_GPIO);
}

void GPIO::writeGPIOA(uint16_t pin_x, int state) {
	if(state == LOW) GPIO_ResetBits(GPIOA, pin_x);
	if(state == HIGH) GPIO_SetBits(GPIOA, pin_x);
}

void GPIO::writeGPIOB(uint16_t pin_x, int state) {
	if(state == LOW) GPIO_ResetBits(GPIOB, pin_x);
	if(state == HIGH) GPIO_SetBits(GPIOB, pin_x);
}

int GPIO::readGPIOA(uint16_t pin_x) {
	return (GPIO_ReadInputData(GPIOA) & pin_x);
}

int GPIO::readGPIOB(uint16_t pin_x) {
	return (GPIO_ReadInputData(GPIOB) & pin_x);	
}
