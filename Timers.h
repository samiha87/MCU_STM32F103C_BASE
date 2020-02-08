/*
 * Timers.h
 *
 *  Created on: Jul 27, 2019
 *      Author: samih
 */

#ifndef __TIMERS_H_
#define __TIMERS_H_

#include "stm32f10x.h"
#include "stm32f10x_tim.h"

class Timers {
public:
	Timers();
	enum enumTimer {
		eTimSecond,
		eTimMikroSecond,
		eTimMilliSecond
	};

	virtual ~Timers();
	// Initializes all timers, 1us, 1ms and 1s
	void init(enumTimer timer);
	void enable(enumTimer timer, bool state);
	void start(enumTimer select, unsigned int interval );
	bool check(enumTimer select);

	void updateTicks(enumTimer select);

	static unsigned int timer_sTicks;
	static unsigned int timer_msTicks;
	static unsigned int timer_usTicks;
	static unsigned int timer_minTicks;
	static unsigned int taskArray[5][2];

	// Delay tasks
	bool delayTask(enumTimer eTimer, int task, unsigned int delay);
	void disableTask(int task);

private:
	// For store tick counts in us
	TIM_TimeBaseInitTypeDef timerInitStructure;
	unsigned char getTimerIRQ(enumTimer timer);
	void enableIRQ(enumTimer timer);

	unsigned int sInterval;
	unsigned int msInterval;
	unsigned int usInterval;

	bool sFlag;
	bool msFlag;
	bool usFlag;

	bool secondTimer;
	void clearTasks();

};

#endif /* TIMERS_H_ */
