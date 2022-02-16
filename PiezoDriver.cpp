/*******************************************************************************
PiezoDriver.cpp

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

#include "PiezoDriver.h"

using namespace SweetMaker;

PiezoDriver::PiezoDriver()
{
	piezoState = LISTENING;
	timerA = PIEZO_DEBOUNCE_TIME;

	eventHandler = NULL;
	knockDetectThreshold = knockDetectThresholdNormal;
	executingCallback = false;
}

void PiezoDriver::configToneGen(IToneGen * tg)
{
	toneGen = tg;
}

void PiezoDriver::configListeningPin(uint8_t pin)
{
    listeningPin = pin;
	startListening();
}

void PiezoDriver::configEventHandler(IEventHandler * handler, uint8_t inst)
{
	eventHandler = handler;
	sourceInst = inst;
}

void PiezoDriver::configKnockDetectThreshold(uint16_t threshold)
{
	knockDetectThreshold = threshold;
}

void PiezoDriver::update(uint16_t elapsedTime_ms)
{
	switch (piezoState)
	{
		case LISTENING:
		{
			if(timerA < elapsedTime_ms)
				timerA = 0;
			else
			{
				timerA -= elapsedTime_ms;
				break;
			}

			if((eventHandler == NULL) || // no callback configured
		       (executingCallback == true))   // must not reenter callback
				break;

			uint16_t vibrationLevel = readVibrationLevel(); // this can take 100us
			if (vibrationLevel > knockDetectThreshold)
			{
				executingCallback = true;
				timerA = PIEZO_DEBOUNCE_TIME;
				eventHandler->handleEvent(
					KNOCK_DETECTED,
					listeningPin,
				    vibrationLevel
				);
				executingCallback = false;
			}
		}
		break;

		case PLAYING_ENDLESS_NOTE:
			break;

		case PLAYING_NOTE:
			if(timerA > elapsedTime_ms)
			{
				timerA -= elapsedTime_ms;
				break;
			}
			startListening();
			piezoState = LISTENING;
			break;

	}
	return;
}

void PiezoDriver::playPeriod_us(uint32_t period_us, uint16_t duration_ms)
{
//	Serial.println(period_us);
	if (period_us == 0)
	{		
		stop();
	    return;
    }
	pinMode(listeningPin, OUTPUT);
	toneGen->playPeriod_us(period_us, 0);

	timerA = duration_ms;
	if(duration_ms == 0)
		piezoState = PLAYING_ENDLESS_NOTE;
	else
		piezoState = PLAYING_NOTE;
}

void PiezoDriver::playFrequency_hz(uint16_t frequency_hz, uint16_t duration_ms)
{
	uint32_t period_us = 0;
	if(frequency_hz != 0)
	  period_us = 1000000 / frequency_hz;
	playPeriod_us(period_us, duration_ms);

}

void PiezoDriver::writeValue(int32_t period_us)
{
	playPeriod_us(period_us, 0);
}

void PiezoDriver::stop()
{
	toneGen->stop();
	pinMode(listeningPin, INPUT_PULLUP);

	piezoState = LISTENING;
	timerA = PIEZO_DEBOUNCE_TIME;
}

void PiezoDriver::setVolume(uint8_t vol)
{
	toneGen->setVolume(vol);
}

uint16_t PiezoDriver::readVibrationLevel()
{
	if(piezoState != LISTENING)
		return 0;

	return(1024 - analogRead(listeningPin));
}

void PiezoDriver::startListening()
{
	toneGen->stop();
	timerA = PIEZO_DEBOUNCE_TIME;
	piezoState = LISTENING;
	pinMode(listeningPin, INPUT_PULLUP);
}


