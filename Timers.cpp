/*
 * Timers.cpp
 *
 *  Created on: Jul 27, 2019
 *      Author: samih
 */
#include "Timers.h"

// Required to use static variables
unsigned int Timers::timer_sTicks = 0;
unsigned int Timers::timer_msTicks = 0;
unsigned int Timers::timer_usTicks = 0;
unsigned int Timers::taskArray[5][2];

Timers::Timers() {
	sFlag = false;
	msFlag = false;
	usFlag = false;

	secondTimer = false;
	usInterval = 0;
	msInterval = 0;
	sInterval = 0;
	clearTasks();
}


Timers::~Timers() {
	// TODO Auto-generated destructor stub
}

/* Update frequency = (TIM_CLOCK/(Prescaler + 1)) / (Period + 1);
 * For an F429 at 180 MHz, TIM on APB2 clock at 180 MHz, on APB1 at 90 MHz
 * Update Frequency = TIM_CLOCK / (P * Q);
 * Where Prescaler = Q - 1; and Period = P - 1;
 * Where On APB2, TIM_CLOCK = 180 MHz
 */


// TIM_CLOCK / (Period * Prescaker) => 72^6 / (36 000 * 2000) => 1s
void Timers::init(enumTimer timer) {
	// Init interrupt timers
	// Get clock frequency
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);

	switch(timer) {

	case eTimSecond:
		/* 1 Second timer
		 * Update frequency = CLOCK / (P * Q)
		 * System clock 72MHz, Period 36000 - 1 and Prescaler 2000
		 * 72MHz / ((36 000 - 1) * 2000) => 1Hz => 1s
		 */
		sFlag = false;

		sInterval = 0;

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		timerInitStructure.TIM_Prescaler = 2000;
		timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		timerInitStructure.TIM_Period = (36000-1); // second
		timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		timerInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM2, &timerInitStructure);
		enable(timer, false);
	break;
	case eTimMilliSecond:

		/* Millisecond timer
		 * Update frequency = CLOCK / (P * Q)
		 * Seconds = 1 / update frequency
		 * System clock 72MHz, Period 101 - 1 and Prescaler 720 - 1
		 * 72MHz / ((101 - 1) * 72) => 10001.4 => 1/1001.4 => 1ms
		 */

		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		timerInitStructure.TIM_Prescaler = (101 - 1);	// Prescale to 1MHz
		timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		timerInitStructure.TIM_Period = (720 - 1);	// 1000 Microseconds equal to 1mS
		timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV2;
		timerInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM3, &timerInitStructure);
	break;
	case eTimMikroSecond:

		/* Mikrosecond timer
		 * Update frequency = CLOCK / (P * Q)
		 * Seconds = 1 / update frequency
		 * System clock 72MHz, Period 101 - 1 and Prescaler 720 - 1
		 * 72MHz / ((72 - 1) * 1) => 10140.8 => 1/10140.8 => 1uS
		 */
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
		timerInitStructure.TIM_Prescaler = 72 - 1;	// Prescale to 1MHz
		timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
		timerInitStructure.TIM_Period = 0;	// Every 1 microsecond cause interrupt
		timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
		timerInitStructure.TIM_RepetitionCounter = 0;
		TIM_TimeBaseInit(TIM4, &timerInitStructure);
	break;
	default:
		break;
	}
}

void Timers::enable(enumTimer timer, bool state) {
	TIM_TypeDef* TIMxL;
	FunctionalState NewState;

	if(state) NewState = ENABLE;
	else NewState = DISABLE;
	switch(timer) {
	case eTimSecond:
		secondTimer = state;
		TIMxL = TIM2;
		break;
	case eTimMilliSecond:
		TIMxL = TIM3;
		break;
	case eTimMikroSecond:
		TIMxL = TIM4;
		break;
	default:
		return;
	}
	TIM_Cmd(TIMxL, NewState);
	TIM_ITConfig(TIMxL, TIM_IT_Update, NewState);
	enableIRQ(timer);
}

unsigned char Timers::getTimerIRQ(enumTimer timer) {

	switch(timer) {
		case eTimSecond:
			return TIM2_IRQn;
			break;
		case eTimMilliSecond:
			return TIM3_IRQn;
		case eTimMikroSecond:
			return TIM3_IRQn;
			break;
		default:
			return 0;
		}
	return 0;
}

void Timers::enableIRQ(enumTimer timer) {
	NVIC_InitTypeDef nvicStructure;
	nvicStructure.NVIC_IRQChannel = getTimerIRQ(timer);
	nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
	nvicStructure.NVIC_IRQChannelSubPriority = 1;
	nvicStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicStructure);
}

void Timers::start(enumTimer select, unsigned int interval) {
	switch(select) {
	case eTimSecond:
		timer_sTicks = 0;
		sInterval = interval;
		break;
	case eTimMilliSecond:
		timer_msTicks = 0;
		msInterval = interval;
		break;
	case eTimMikroSecond:
		timer_usTicks = 0;
		usInterval = interval;
		break;
	default:
		return;
	}
	enable(select, true);
}

bool Timers::check(enumTimer select) {
	switch(select) {
	case eTimSecond:
		if(!secondTimer) return false;
		if(timer_sTicks >= sInterval) {
			timer_sTicks = 0;
			return true;
		}
		break;
	case eTimMilliSecond:
		if(timer_msTicks >= msInterval) {
			timer_msTicks = 0;
			return true;
		}
		break;
	case eTimMikroSecond:
		timer_usTicks = 0;
		if(timer_usTicks >= usInterval) {
			timer_usTicks = 0;
			return true;
		}
		break;
	default:
		return false;
	}
	return false;
}

// Seconds
extern "C" void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        Timers::timer_sTicks++;
        // Check if tasks enabled
        for(int i = 0; i < 10; i++) {
        	if(Timers::taskArray[i][1] == ENABLE) {
        		Timers::taskArray[i][0]++;
        	}
        }
        //Timers::updateTicks(Timers::eTimSecond);
    }
}

// Milliseconds
extern "C" void TIM3_IRQHandler()
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        Timers::timer_msTicks++;
    }
}

// Mikro seconds
extern "C" void TIM4_IRQHandler()
{
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
        Timers::timer_usTicks++;
    }
}

bool Timers::delayTask(enumTimer eTimer, int task, unsigned int delay) {
	if(Timers::taskArray[task][1] == ENABLE) {
		if (Timers::taskArray[task][0] >= delay) {
			Timers::taskArray[task][0] = 0;
			return true;
		}
		else return false;
	} else {
		Timers::taskArray[task][0] = 0;
		Timers::taskArray[task][1] = ENABLE;
		return true;
	}
}

void Timers::disableTask(int task) {
	Timers::taskArray[task][1] = DISABLE;
}

void Timers::clearTasks() {

	for(int i = 0; i < 5; i++ ) {
		Timers::taskArray[i][0] = 0;
		Timers::taskArray[i][1] = DISABLE;
	}

}
