/*******************************************************************************
SwitchDriver.h Takes Arduino pin id, provides helpful button API.

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


#ifndef __SWITCH_DRIVER_H__
#define __SWITCH_DRIVER_H__

#include "IEventHandler.h"
#include "Updater.h"

namespace SweetMaker {

	class SwitchDriver: private AutoUpdate {
	public:
		typedef enum SwitchEvent {
			SWITCH_TURNED_ON = (IEventHandler::SWITCH_EVENTS + 0),
			SWITCH_TURNED_OFF = (IEventHandler::SWITCH_EVENTS + 1),
			SWITCH_HELD_ON = (IEventHandler::SWITCH_EVENTS + 2),
			SWITCH_STILL_HELD_ON = (IEventHandler::SWITCH_EVENTS + 3)
		}SWITCH_EVENT;

		typedef enum SwitchState {
			STATE_OFF = IEventHandler::SWITCH_EVENTS + 0,
			STATE_ON = IEventHandler::SWITCH_EVENTS + 1,
			STATE_LONG_ON = IEventHandler::SWITCH_EVENTS + 2
		}SWITCH_STATE;

		SwitchDriver();
		void config(uint8_t pinNum, uint16_t inactiveState);		
        void configEventHandler(IEventHandler::EventHandler handler);
        void configEventHandler(IEventHandler * handler);
		void configDebounceTimer_ms(uint16_t time_ms);
		void configHoldDownTimer_ms(uint16_t time_ms);
		
		void update(uint16_t elapsedTime_ms);
		
		SWITCH_STATE getState();

	private:
		uint8_t pinNum;
		uint16_t debouncePeriod_ms;
		uint16_t holdDownPeriod_ms;

		SWITCH_STATE state;
		uint8_t inactivePinState;
		uint16_t debounceTimer;
		uint16_t holdDownTimer;
		uint16_t holdDownCount;

		void dispatchEvent(SWITCH_EVENT eventId, uint16_t eventInfo);
		IEventHandler::EventHandler eventHandlerCallback;
		IEventHandler * eventHandler;
	};

}

#endif // _SWITCH_DRIVER_H