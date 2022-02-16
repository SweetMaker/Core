/*******************************************************************************
PiezoDriver.h Takes Arduino Pin ID; Provides Knock Detection and Tune Generation

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

#ifndef __PIEZO_DRIVER_H__
#define __PIEZO_DRIVER_H__

#include "Arduino.h"
#include "SigGen.h"
#include "IEventHandler.h"
#include "IToneGen.h"
#include "Updater.h"

namespace SweetMaker
{

class PiezoDriver : public IToneGen, private AutoUpdate
{
public:
	typedef enum
	{
	    LISTENING = 0,
	    PLAYING_NOTE = 1,
	    PLAYING_ENDLESS_NOTE = 2
	} PIEZZO_STATE;

	typedef enum
	{
	    KNOCK_DETECTED = IEventHandler::PIEZO_EVENTS + 0
	} PIEZO_EVENT;

	static const uint16_t knockDetectThresholdNormal = 40;
	static const uint16_t knockDetectThresholdSensitive = 20;
	static const uint16_t knockDetectThresholdInsensitive = 60;

#define PIEZO_DEBOUNCE_TIME (30)

	PiezoDriver();
	void configToneGen(IToneGen * toneGen);
	void configListeningPin(uint8_t listeningPin);
	void configEventHandler(IEventHandler * eventHandler, uint8_t inst);
	void configKnockDetectThreshold(uint16_t threshold);
	void update(uint16_t elapsedTime_ms);

	void playPeriod_us(uint32_t period_us, uint16_t duration_ms);
	void playFrequency_hz(uint16_t frequency_hz, uint16_t duration_ms);

	void writeValue(int32_t period_us);

	void stop(void);
	void setVolume(uint8_t volume);

private:
	uint16_t readVibrationLevel();
	void startListening();

	IToneGen * toneGen;
	uint8_t listeningPin;

	PIEZZO_STATE piezoState;
	uint16_t timerA;

	IEventHandler * eventHandler;
	uint8_t sourceInst;

	uint16_t knockDetectThreshold;
	bool executingCallback;
};

}

#endif
