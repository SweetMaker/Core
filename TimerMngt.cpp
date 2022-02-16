/*******************************************************************************
TimerMngt.cpp

Copyright(C) 2015-2016  Howard James May

This file is part of the SweetMaker SDK

The SweetMaker SDK is free software: you can redistribute it and / or
modify it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

The SweetMaker SDK is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.

Contact me at sweet.maker@outlook.com
********************************************************************************
Release     Date                        Change Description
--------|-------------|--------------------------------------------------------|
  1      24-Feb-2016   Initial release
*******************************************************************************/
#include <Arduino.h>
#include "TimerMngt.h"
#include "EventMngr.h"

using namespace SweetMaker;

#ifndef NULL
#define NULL 0
#endif

TimerTickMngt * TimerTickMngt::getTimerMngt()
{
	static TimerTickMngt * instance = NULL;
	if (instance == NULL)
		instance = new TimerTickMngt();
	return instance;
}

TimerTickMngt::TimerTickMngt()
{
	eventHandlerFunction = 0;
	eventHandlerObject = 0;
	eventSourceRef = 0;
	
	time_counter_ms = 0;
	time_counter_100ms = 0;
	time_counter_1s = 0;
}

void TimerTickMngt::update(uint16_t elapsedTime_ms)
{
	time_counter_ms += elapsedTime_ms;

	EventMngr::getMngr()->handleEvent(TIMER_TICK_UPDATE, 0xff, elapsedTime_ms);
	while(time_counter_ms >= 100)
	{
		EventMngr::getMngr()->handleEvent(TIMER_TICK_100MS, 0xff, time_counter_ms);

		time_counter_ms -= 100;
		time_counter_100ms++;
		
		if(time_counter_100ms >= 10)
		{
			EventMngr::getMngr()->handleEvent(TIMER_TICK_S, 0xff, time_counter_1s);
			time_counter_100ms = 0;
			time_counter_1s++;

			if(time_counter_1s >= 10)
			{
				EventMngr::getMngr()->handleEvent(TIMER_TICK_10S, 0xff, time_counter_1s);
				time_counter_1s = 0;
			}
		}
	}
}

FreqGen::FreqGen()
{
	initialise();
	AutoUpdate::removeFromUpdater();
}

FreqGen::FreqGen(uint16_t freq_hz, uint16_t ref)
{
	initialise();
	startFreqGen(freq_hz, ref);
}

void FreqGen::initialise()
{
	countPeriod_ms = 0;
	eventCount = 0;
	frequency_Hz = 0;
	eventInfo = 0xffff;
}

int FreqGen::startFreqGen(uint16_t freq_hz, uint16_t ref)
{
	if (!isRunning())
		AutoUpdate::addToUpdater();

	this->countPeriod_ms = 0;
	this->eventCount = 0;
	this->frequency_Hz = freq_hz;
	this->eventInfo = ref;

	return 0;
}

int FreqGen::stopFreqGen(void)
{
	if (isRunning())
		AutoUpdate::removeFromUpdater();
	this->frequency_Hz = 0;
	return -1;
}

bool FreqGen::isRunning()
{
	if(this->frequency_Hz == 0)
	  return (false);
	return (true);
}

void FreqGen::update(uint16_t elapsedTime_ms)
{
	uint32_t expectedEventCount_m;
	countPeriod_ms += elapsedTime_ms;
	expectedEventCount_m = (uint32_t)frequency_Hz * (uint32_t)countPeriod_ms;

	while ((uint32_t)((uint32_t)(eventCount + 1) * (uint32_t)1000) <= expectedEventCount_m)
	{
		eventCount++;
		EventMngr::getMngr()->handleEvent(TimerTickMngt::TIMER_FREQ_GEN, 0xff, eventInfo);
	}

	if ((expectedEventCount_m >= 4000000000) || (countPeriod_ms >= 60000) || (eventCount >= 4000))
	{
		uint32_t usedTime_ms = (uint32_t)((eventCount - 1)* (uint32_t)1000) / frequency_Hz;
		countPeriod_ms -= usedTime_ms;
		eventCount = 1;
	}
}

Timer::Timer()
{
	initialise();
	AutoUpdate::removeFromUpdater();
}

Timer::Timer(uint16_t duration_ms, uint16_t ref)
{
	initialise();
	this->countDown_ms = duration_ms;
	this->eventInfo = ref;
}

void Timer::initialise()
{
	countDown_ms = 0;
	eventInfo = 0xffff;
}

int Timer::startTimer(uint16_t duration, uint16_t ref)
{

	if (!this->isRunning())
		AutoUpdate::addToUpdater();

	this->countDown_ms = duration;
	this->eventInfo = ref;

	return (0);
}

int Timer::stopTimer(void)
{
	if (this->isRunning())
		AutoUpdate::removeFromUpdater();
	this->countDown_ms = 0;
	return (0);
}

bool Timer::isRunning()
{
	if (this->countDown_ms)
		return true;
	return false;
}

void Timer::update(uint16_t elapsedTime_ms)
{
	if (elapsedTime_ms >= countDown_ms)
	{
		countDown_ms = 0;
		AutoUpdate::removeFromUpdater();
		EventMngr::getMngr()->handleEvent(TimerTickMngt::TIMER_EXPIRED, 0xff, eventInfo);
	}
	else
	{
		countDown_ms -= elapsedTime_ms;
	}
}
