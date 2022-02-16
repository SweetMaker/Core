/*******************************************************************************
LedDriver.h - Takes IO Pin and provides easy API for controlling LED

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


#ifndef __LED_DRIVER_H__
#define __LED_DRIVER_H__

#include <Arduino.h>
#include "ISigInput.h"

namespace SweetMaker
{

	class LedDriver : public ISigInput
	{
	public:
		LedDriver();
		void configPin(uint8_t pinNum);
		void configLevelling(uint8_t level);

		void turnOn();
		void turnOn(uint8_t brightness);

		void writeValue(int32_t value);

		void turnOff();

	private:
		uint8_t ledPin;
		uint8_t balanceLevel;

		void calculateAndSetOutput(uint8_t brightness);
		uint8_t perceivedBrightnessCorrection(uint8_t requestedBrightness);
	};

	/*
	* Regression Test Code: Only conditionally included
	*/
#ifdef SWEETMAKER_REGTEST
#include "SigGen.h"
	class LedDriverRegTest : public TestSet
	{
	public:
		LedDriverRegTest();
		void configLedDriver(LedDriver* ledDriver, SigGen * sigGen);
		void printName();
		void handleEvent(uint16_t eventId, uint8_t srcRef, uint16_t eventInfo);

	private:
		typedef enum {
			STATE_IDLE = 0,
			STATE_T1_CHECK_LED_ON  = 1,
			STATE_T1_CHECK_LED_OFF = 2,
			STATE_T1_CHECK_SMOOTH_BRIGHTNESS = 3,
			STATE_T2_CHECK_HEARTBEAT = 4,
			STATE_FINISHED = 10
		}REG_TST_STATE;

		LedDriver * ledDriver;
		SigGen * sigGen;
	};

#endif 

}

#endif



