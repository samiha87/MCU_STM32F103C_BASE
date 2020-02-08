#include "USART.h"
#include "defines.h"

GPIO_InitTypeDef GPIO_InitStructureUSART;
USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;

char USART::buffer1[UART_BUFFER_SIZE];
char USART::buffer2[UART_BUFFER_SIZE];
char USART::buffer3[UART_BUFFER_SIZE];

bool USART::rxFlag1 = false;
bool USART::rxFlag2 = false;
bool USART::rxFlag3 = false;

int USART::bPoll1 = 0;
int USART::bPoll2 = 0;
int USART::bPoll3 = 0;

// Ensure these are casted only once
extern "C" void USART1_IRQHandler(void) {
	// Confirm source, reading DR clears
	if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET) {
		if(USART::bPoll1 > UART_BUFFER_SIZE) USART::bPoll1 = 0;
		USART::buffer1[USART::bPoll1++] = USART_ReceiveData(USART1);
		USART::rxFlag1 = true;
	}
}

extern "C" void USART2_IRQHandler(void) {
	// Confirm source, reading DR clears
	if (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) != RESET) {
		if(USART::bPoll2 > UART_BUFFER_SIZE) USART::bPoll2 = 0;
		USART::buffer2[USART::bPoll2++] = USART_ReceiveData(USART2);
		USART::rxFlag2 = true;
	}
}

extern "C" void USART3_IRQHandler(void) {
	// Confirm source, reading DR clears
	if (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) != RESET) {
		if(USART::bPoll3 > UART_BUFFER_SIZE) USART::bPoll3 = 0;
		USART::buffer3[USART::bPoll3++] = USART_ReceiveData(USART3);
		USART::rxFlag3 = true;
	}
}

USART::USART(enumUSART eUSART, uint32_t baudrate) {
	instanceUSART = eUSART;
	USART::bPoll1 = 0;
	USART::bPoll2 = 0;
	USART::bPoll3 = 0;
	USART::rxFlag1 = false;
	USART::rxFlag2 = false;
	USART::rxFlag3 = false;

	switch(instanceUSART) {
	case eUSART1:
		// Enable peripheral clocs for USAR1 on GPIOA
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

		/* Configure PA9 and PA10 as USART1 TX/RX */
		GPIO_InitStructureUSART.GPIO_Pin = GPIO_Pin_9;	// USART1 TX
		GPIO_InitStructureUSART.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructureUSART.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructureUSART);

		// PA10 floating input
		GPIO_InitStructureUSART.GPIO_Pin = GPIO_Pin_10;	// USART1 RX
		GPIO_InitStructureUSART.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructureUSART);

		// Configure and initialize usart
		USART_InitStructure.USART_BaudRate = baudrate;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		USART_Init(USART1, &USART_InitStructure);
		break;
	case eUSART2:
		// Enable peripheral clocs for USAR1 on GPIOA
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		/* Configure PA9 and PA10 as USART1 TX/RX */
		GPIO_InitStructureUSART.GPIO_Pin = GPIO_Pin_2;	// PA2, Pin 12 TX,
		GPIO_InitStructureUSART.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructureUSART.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOA, &GPIO_InitStructureUSART);

		// PA10 floating input
		GPIO_InitStructureUSART.GPIO_Pin = GPIO_Pin_3; // PA3, Pin 13 RX
		GPIO_InitStructureUSART.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOA, &GPIO_InitStructureUSART);

		// Configure and initialize usart
		USART_InitStructure.USART_BaudRate = baudrate;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		USART_Init(USART2, &USART_InitStructure);
		break;
	case eUSART3:
		// Enable peripheral clocs for USAR3 on GPIOA
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

		/* Configure PB10 and PB11 as USART3 TX/RX */
		GPIO_InitStructureUSART.GPIO_Pin = GPIO_Pin_10;	// PB10, Pin 10 TX,
		GPIO_InitStructureUSART.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructureUSART.GPIO_Mode = GPIO_Mode_AF_PP;
		GPIO_Init(GPIOB, &GPIO_InitStructureUSART);
		// PA10 floating input
		GPIO_InitStructureUSART.GPIO_Pin = GPIO_Pin_11; // PB11, Pin 11 RX
		GPIO_InitStructureUSART.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(GPIOB, &GPIO_InitStructureUSART);

		// Configure and initialize usart
		USART_InitStructure.USART_BaudRate = baudrate;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		USART_Init(USART3, &USART_InitStructure);
		break;
	}
}

void USART::enable() {
	switch(instanceUSART) {
	case eUSART1:
		USART_Cmd(USART1, ENABLE);
		break;
	case eUSART2:
		USART_Cmd(USART2, ENABLE);
		break;
	case eUSART3:
		USART_Cmd(USART3, ENABLE);
		break;
	}
}

// Enable the USART RX Interrupt
void USART::enableIRQ() {
	switch(instanceUSART) {
	case eUSART1:
		// Enable the USART RX Interrupt
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		break;
	case eUSART2:
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		break;
	case eUSART3:
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		break;
	}
}

void USART::disable() {
	switch(instanceUSART) {
		case eUSART1:
			USART_Cmd(USART1, DISABLE);
			break;
		case eUSART2:
			USART_Cmd(USART2, DISABLE);
			break;
		case eUSART3:
			USART_Cmd(USART3, DISABLE);
			break;
	};
}

void USART::send(const char *input, const int len) {
	int poll = 0;
	switch(instanceUSART) {
	case eUSART1:
		if(busy()) return;
			for(poll = 0; poll < len; poll++) {
				USART_SendData(USART1, input[poll]);
				while(busy() == TRUE);
		}
		break;
	case eUSART2:
		if(busy()) return;
		for(poll = 0; poll < len; poll++) {
			USART_SendData(USART2, input[poll]);
			while(busy() == TRUE);
		}
		break;
	case eUSART3:
		if(busy()) return;
		for(poll = 0; poll < len; poll++) {
			USART_SendData(USART3, input[poll]);
			while(busy() == TRUE);
		}
		break;
	};
}

bool USART::available(){
	bool state = false;
	switch(instanceUSART) {
	case eUSART1:
		state = USART::rxFlag1;
		break;
	case eUSART2:
		state = USART::rxFlag2;
		break;
	case eUSART3:
		state = USART::rxFlag3;
		break;
	};
	return state;
}

void USART::read(char *output, const int maxLen) {
	int poll = 0;
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) > 0) {
		if(poll >= maxLen) break;
		output[poll++] = USART_ReceiveData(USART1);
	}
}

void USART::addBuffer(char input) {
	switch(instanceUSART) {
	case eUSART1:
		USART::buffer1[USART::bPoll1++] = input;
		break;
	case eUSART2:
		USART::buffer2[USART::bPoll2++] = input;
		break;
	case eUSART3:
		USART::buffer3[USART::bPoll3++] = input;
		break;
	};

}

void USART::setRxFlag(bool input) {
	switch(instanceUSART) {
	case eUSART1:
		USART::rxFlag1 = input;
		break;
	case eUSART2:
		USART::rxFlag2 = input;
		break;
	case eUSART3:
		USART::rxFlag3 = input;
		break;
	};
}

bool USART::getRxFlag() {
	bool flag = false;
	switch(instanceUSART) {
		case eUSART1:
			flag = USART::rxFlag1;
			break;
		case eUSART2:
			flag = USART::rxFlag2;
			break;
		case eUSART3:
			flag = USART::rxFlag3;
			break;
		};
	return flag;
}

int USART::getCount() {
	int toReturn = 0;
	switch(instanceUSART) {
		case eUSART1:
			toReturn = USART::bPoll1;
			break;
		case eUSART2:
			toReturn = USART::bPoll2;
			break;
		case eUSART3:
			toReturn = USART::bPoll3;
			break;
		};
	return toReturn;
}

// This will also reset buffer;
bool USART::getBuffer(char *output, const int maxLen) {
	bool state = false;
	switch(instanceUSART) {
	case eUSART1:
		state = getBuffer1(output, maxLen);
		break;
	case eUSART2:
		state = getBuffer2(output, maxLen);
		break;
	case eUSART3:
		state = getBuffer3(output, maxLen);
		break;
	};
	return state;
};

bool USART::getBuffer1(char *output, const int maxLen) {
	if(!USART::rxFlag1) return false;
	for(int i = 0; i < maxLen; i++) {
		if(i >= UART_BUFFER_SIZE) return false;	// Check that Iterator doesn't exceed buffer size
		if(i >= USART::bPoll1) return false;	// Check that Iterator doesn't exceed received message size
		output[i] = USART::buffer1[i];
	}
	resetBuffer();
	return true;
}

bool USART::getBuffer2(char *output, const int maxLen) {
	if(!USART::rxFlag2) return false;
	for(int i = 0; i < maxLen; i++) {
		if(i >= UART_BUFFER_SIZE) return false;	// Check that Iterator doesn't exceed buffer size
		if(i >= USART::bPoll2) return false;	// Check that Iterator doesn't exceed received message size
		output[i] = USART::buffer2[i];
	}
	resetBuffer();
	return true;
}

bool USART::getBuffer3(char *output, const int maxLen) {
	if(!USART::rxFlag3) return false;
	for(int i = 0; i < maxLen; i++) {
		if(i >= UART_BUFFER_SIZE) return false;	// Check that Iterator doesn't exceed buffer size
		if(i >= USART::bPoll3) return false;	// Check that Iterator doesn't exceed received message size
		output[i] = USART::buffer3[i];
	}
	resetBuffer();
	return true;
}

void USART::resetBuffer() {
	switch(instanceUSART) {
	case eUSART1:
		rxFlag1 = false;
		for(int i = 0; i < UART_BUFFER_SIZE; i++) {
			buffer1[i] = 0;
		}
		bPoll1 = 0;
		break;

	case eUSART2:
		rxFlag2 = false;
		for(int i = 0; i < UART_BUFFER_SIZE; i++) {
			buffer2[i] = 0;
		}
		bPoll2 = 0;
		break;
	case eUSART3:
		rxFlag3 = false;
		for(int i = 0; i < UART_BUFFER_SIZE; i++) {
			buffer3[i] = 0;
		}
		bPoll3 = 0;
		break;
	};

}

bool USART::busy() {
	bool state = false;
	switch(instanceUSART) {
		case eUSART1:
			if(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) state = true;
			else state = false;
			break;
		case eUSART2:
			if(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET) state = true;
			else state = false;
			break;
		case eUSART3:
			if(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET) state = true;
			else state = false;
			break;
	};
	return state;
}
