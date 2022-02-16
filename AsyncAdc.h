/*******************************************************************************
  AsyncAdc.h Asynchronous Analogue To Digital Convertion

  Copyright(C) 2018  Howard James May

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
   1      09-Oct-2018   Initial release
*******************************************************************************/

#ifdef ARDUINO_AVR_NANO

#ifndef __ASYNC_ADC_H__
#define __ASYNC_ADC_H__

#include <Arduino.h>
#include "AvrAdc.h"

namespace SweetMaker {
	class AsyncAdcMngr {
	public:
		static const int INSUFFICIENT_CHANNELS = -1;
		static const int PIN_ALREADY_ENABLED = -2;
		static const int PIN_DISABLED = -3;

		AsyncAdcMngr(uint8_t numChannels);
		static int initAsyncAdcMngr(uint8_t numChannels);
		static int enablePin(uint8_t pinNum);
		static int readPin(uint8_t pinNum);
		static int disablePin(uint8_t pinNum);
		static void interruptHandler(void);

	private:
		volatile static uint8_t maxChannels;
		volatile static int8_t currentChannel;

		typedef enum {
			DISABLED = 0,      // Not currently being read
			ENABLED_STALE = 1, // Just enabled but not yet with a valid reading
			ENABLED = 2         // Enabled and with valid readings
		}CHANNEL_STATE;

		typedef struct channel {
			uint8_t pinNum;
			CHANNEL_STATE state;
			uint16_t lastReading;
			AvrAdc::ADC_PRECISION precision;
		}ASYN_ADC_CHANNEL;

		static volatile ASYN_ADC_CHANNEL * channels;

		static int startAsyncReading();
		static int stopAsyncReading();
		static int8_t getNextChannel();
	};
}
#endif

#endif