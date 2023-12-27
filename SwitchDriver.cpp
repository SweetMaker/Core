/*******************************************************************************
SwitchDriver.cpp Takes Arduino pin id, provides helpful button API.

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
2      05-Feb-2017   Fixed bug where heldDownCount was not reset
*******************************************************************************/



#include <Arduino.h>
#include "SwitchDriver.h"
#include "TimerMngt.h"

using namespace SweetMaker;


SwitchDriver::SwitchDriver()
{
	inactivePinState = LOW;
	pinNum = 0;
	eventHandlerCallback = NULL;
	eventHandler = NULL;

	debouncePeriod_ms = 10;
	holdDownPeriod_ms = 1000;

	state = STATE_OFF;
	debounceTimer = 0;
}

void SwitchDriver::config(uint8_t _pinNum, uint16_t inactiveState)
{
	pinNum = _pinNum;
	inactivePinState = inactiveState;
	pinMode(pinNum, INPUT_PULLUP);
}

void SwitchDriver::configEventHandler(IEventHandler::EventHandler handler)
{
	eventHandlerCallback = handler;
}

void SwitchDriver::configEventHandler(IEventHandler * handler)
{
	eventHandler = handler;
}

void SwitchDriver::configDebounceTimer_ms(uint16_t time_ms)
{
	debouncePeriod_ms = time_ms;
}

void SwitchDriver::configHoldDownTimer_ms(uint16_t time_ms)
{
	holdDownPeriod_ms = time_ms;
}

SwitchDriver::SWITCH_STATE SwitchDriver::getState()
{
	return state;
}


void SwitchDriver::update(uint16_t elapsedTime_ms)
{
	uint8_t pinState = digitalRead(pinNum);

	switch (state)
	{
		case STATE_OFF:
		{
			if (pinState == inactivePinState)
			{
				debounceTimer = debouncePeriod_ms;
				break;
			}

			if(elapsedTime_ms < debounceTimer)
			{
				debounceTimer -= elapsedTime_ms;
				break;
			}

			state = STATE_ON;
			holdDownCount = 0;
			debounceTimer = debouncePeriod_ms;
			holdDownTimer = holdDownPeriod_ms;
			dispatchEvent(SWITCH_TURNED_ON,0);
		}
		break;

		case STATE_LONG_ON:
		case STATE_ON:
		{
			if (pinState != inactivePinState)
			{
				debounceTimer = debouncePeriod_ms;

				if(elapsedTime_ms < holdDownTimer)
					holdDownTimer -= elapsedTime_ms;
				else
				{
					if (state == STATE_LONG_ON)
					{
						holdDownCount++;
						dispatchEvent(SWITCH_STILL_HELD_ON, holdDownCount);
					}
					else
					{
						holdDownCount = 0;
						dispatchEvent(SWITCH_HELD_ON, 0);
					}
					state = STATE_LONG_ON;
					holdDownTimer = holdDownPeriod_ms;
				}
				break;
			}

			if(elapsedTime_ms < debounceTimer)
			{
				debounceTimer -= elapsedTime_ms;
				break;
			}

			state = STATE_OFF;
			debounceTimer = debouncePeriod_ms;
			holdDownTimer = holdDownPeriod_ms;
			dispatchEvent(SWITCH_TURNED_OFF,0);
		}
		break;

		default:
			break;
	}
}

void SwitchDriver::dispatchEvent(SWITCH_EVENT eventId, uint16_t eventInfo)
{
	if (eventHandlerCallback)
		eventHandlerCallback(eventId, pinNum, eventInfo);

	if (eventHandler)
		eventHandler->handleEvent(eventId, pinNum, eventInfo);
}


