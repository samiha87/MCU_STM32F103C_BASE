#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_usart.h"
#include "defines.h"

class USART {

public:
	enum enumUSART {
		eUSART1,
		eUSART2,
		eUSART3
	};
	USART(enumUSART eUSART, uint32_t baudrate);
	void enable();
	void enableIRQ();
	void disable();
	void send(const char *input, const int);
	bool available();
	void read(char *output, const int maxLen);
	void addBuffer(char input);
	void setRxFlag(bool input);
	bool getRxFlag();
	int getCount();
	// This will also reset buffer;
	bool getBuffer(char *output, const int maxLen);
	void resetBuffer();

	// Static functions necessary to be static to use interrupts
	static char buffer1[UART_BUFFER_SIZE];
	static char buffer2[UART_BUFFER_SIZE];
	static char buffer3[UART_BUFFER_SIZE];

	static bool rxFlag1;
	static bool rxFlag2;
	static bool rxFlag3;

	static int bPoll1;
	static int bPoll2;
	static int bPoll3;
private:
	enumUSART instanceUSART;
	bool busy();
	bool getBuffer1(char *output, const int maxLen);
	bool getBuffer2(char *output, const int maxLen);
	bool getBuffer3(char *output, const int maxLen);
};


#endif
