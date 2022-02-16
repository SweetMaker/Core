/*******************************************************************************
    TimerMngt.h

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

#ifndef __TIMER_MNGT_H__
#define __TIMER_MNGT_H__

#include <stdint.h>
#include "IEventHandler.h"
#include "Updater.h"

namespace SweetMaker
{
	/*
	 * Forward Declaration
	 */
	class Timer;
	class FreqGen;

	class TimerTickMngt : AutoUpdate
	{
		friend class Timer;
		friend class FreqGen;
	public:
		static TimerTickMngt * getTimerMngt();

		typedef enum
		{
			TIMER_TICK_UPDATE = IEventHandler::TIMER_EVENTS + 0,
			TIMER_TICK_100MS = IEventHandler::TIMER_EVENTS + 1,
			TIMER_TICK_S = IEventHandler::TIMER_EVENTS + 2,
			TIMER_TICK_10S = IEventHandler::TIMER_EVENTS + 3,
			TIMER_EXPIRED = IEventHandler::TIMER_EVENTS + 4,
			TIMER_STOPPED = IEventHandler::TIMER_EVENTS + 5,
			TIMER_STARTED = IEventHandler::TIMER_EVENTS + 6,
			TIMER_RESTARTED = IEventHandler::TIMER_EVENTS + 7,
			TIMER_FREQ_GEN = IEventHandler::TIMER_EVENTS + 8
		} TIMER_EVENTS;

		void update(uint16_t elapsedTime_ms);

	private:
		TimerTickMngt();
		uint16_t time_counter_ms;
		uint8_t time_counter_100ms;
		uint8_t time_counter_1s;

		IEventHandler * eventHandlerObject;
		IEventHandler::EventHandler eventHandlerFunction;
		uint8_t eventSourceRef;
	};

	class Timer : AutoUpdate
	{
	public:
		Timer();
		Timer(uint16_t duration_ms, uint16_t ref);
		int startTimer(uint16_t duration_ms, uint16_t eventInfo);
		int stopTimer(void);
 		bool isRunning();

	protected:
		void update(uint16_t elapsedTime_ms);

	private:
		void initialise();
		uint16_t countDown_ms;
		uint16_t eventInfo;
	};

	class FreqGen : AutoUpdate
	{
	public :
		FreqGen();
		FreqGen(uint16_t freq_hz, uint16_t ref);
		int startFreqGen(uint16_t freq_hz, uint16_t ref);
		int stopFreqGen();
		bool isRunning();

	protected :
		void update(uint16_t elapsedTime_ms);

	public:
		void initialise();
		uint16_t countPeriod_ms;
		uint16_t eventCount;
		uint16_t frequency_Hz;
		uint16_t eventInfo;
	};
}

#endif
