/*******************************************************************************
IEventHandler.h Provides Interface for handling events

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

#ifndef __I_EVENT_HANDLER_H__
#define __I_EVENT_HANDLER_H__
#include <stdint.h>

namespace SweetMaker {
	class IEventHandler {
		
	public:
		typedef enum {
			SWITCH_EVENTS =         0x0000,
			SIG_GEN_EVENTS =        0x0010,
			LED_EVENTS = 	        0x0020,
			TONE_GEN_EVENTS = 		0x0030,
			PIEZO_EVENTS =      	0x0040,
			TUNE_PLAYER_EVENTS = 	0x0050,
			TIMER_EVENTS =          0x0060,
			MOTION_SENSOR_EVENTS =  0x0070,
			BLE_MIDI_EVENTS =       0x0080,
			LED_STRIP_EVENTS =      0x0090,
			LIBRARY =               0x8000,
			USER =                  0xF000
		}EVENT_OFFSET;
		
		typedef void(*EventHandler)(
			uint16_t eventId,        // The event which has occured. E.g. Switch turned ON
			uint8_t sourceReference, // You might have multiple LEDs, this says which one
			uint16_t eventInfo);     // Information about the event if applicable

		virtual void handleEvent(
			uint16_t eventId,
			uint8_t sourceReference,
			uint16_t eventInfo) = 0;
	};
}
#endif


