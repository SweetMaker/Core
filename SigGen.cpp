/*******************************************************************************
SigGen.cpp Takes data samples; provides time varying signal.

	Copyright(C) 2015-2017  Howard James May

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
   2      20-Feb-2017   Removed eventHandler in deference to global reference
   3      22-Feb-2017   Changed to using PROGMEM
   4      31-Mar-2017   Removed static declaration on sineWave255
                        Removed use of enum label causing OS-X to complain
*******************************************************************************/

#include <Arduino.h>
#include "SigGen.h"
#include "EventMngr.h"

using namespace SweetMaker;

const SigGen::SAMPLE SweetMaker::sineWave255[] PROGMEM = { 128,152,176,198,218,234,245,253,255,253,245,234,218,198,176,152,128,103,79,57,37,21,10,2,0,2,10,21,37,57,79,103 };

SigGen::SigGen()
{
	initialise();
}

SigGen::SigGen(
	const SAMPLE * samples, 
	uint8_t numSamples, 
	uint32_t period_ms, 
	uint8_t options
)
{
	initialise();
	configSamples(samples, numSamples, period_ms, options);
}

void SigGen::initialise()
{
	sigGenState = UNCONFIGURED;
	currentIndex = 0;
	signalOutput = NULL;
}

void SigGen::configSamples(const SAMPLE * samples, uint8_t num, uint32_t period_ms, uint8_t options)
{
	this->options = options;
	sampleArray = samples;
	numSamples = num;
	configPeriod_ms(period_ms);
	currentIndex = 0;
	timeInHand_ms = 0;
	cyclesRemaining = 0;
	sigGenState = STOPPED;
}

void SigGen::configOutput(ISigInput * output)
{
	signalOutput = output;
}

void SigGen::configEventRef(uint8_t ref)
{
	eventSourceRef = ref;
}

void SigGen::configOptions(uint8_t opt)
{
	options = opt;
}

void SigGen::configPeriod_ms(uint32_t period_ms)
{
	/* 
	 * SigGen starts at the first sample value and ends at the last leaving
	 * numSamples -1 'sections'. Except if interpolation is disabled in which
	 * case there are numSample 'sections'
	 */
	if(options & INTERPOLATE_OFF)
		this->samplePeriod_ms = period_ms / numSamples;
	else
		this->samplePeriod_ms = period_ms / (numSamples - 1);

	/* Cp is used to speed up interpolation */
	Cp = (0x10000) * (numSamples-1) / period_ms;

	/* Can't allow samplePeriod_ms to be zero */
	if (this->samplePeriod_ms == 0)
		this->samplePeriod_ms = 1;
}

/*
* Calculates the new index value and remaining time in hand, and then gets the signal value updated
*/
void SigGen::update(uint16_t elapsedTime_ms)
{
	uint16_t maxSampleId;
	
	if (sigGenState == WAITING_TO_START)
	{
		if (timeInHand_ms >= elapsedTime_ms)
		{
			timeInHand_ms -= elapsedTime_ms;
			return;
		}

		elapsedTime_ms -= timeInHand_ms;
		timeInHand_ms = 0;
		sigGenState = RUNNING;
		broadcastEvent(SIG_GEN_STARTED, 0);
	}

	if (sigGenState != RUNNING)
	  return;

	timeInHand_ms += elapsedTime_ms;

	while (timeInHand_ms >= samplePeriod_ms)
	{
		currentIndex++;
		timeInHand_ms -= samplePeriod_ms;
	}

	maxSampleId = numSamples - 1;
	if (options & INTERPOLATE_OFF)
		maxSampleId++;

	while (currentIndex >= maxSampleId)
	{
		/*
		 * We have completed this cycle
		 */
		if (cyclesRemaining == 0)
		{
			currentIndex -= maxSampleId;
		}
		else if (cyclesRemaining > 1)
		{
			cyclesRemaining--;
			currentIndex -= maxSampleId;
		}
		else
		{
			sigGenState = STOPPED;
			cyclesRemaining = 0;
			currentIndex = 0;
			timeInHand_ms = 0;

			if (!(options & DONT_FINISH_ON_ZERO) && signalOutput != NULL)
				signalOutput->writeValue(0);

			broadcastEvent(SIG_GEN_FINISHED, 0);
			return;
		}
	}
	if (options & INTERPOLATE_OFF)
		currentSigValue = getSample(currentIndex);
	else
		currentSigValue = interpolate();

	if (signalOutput != NULL)
	{
		signalOutput->writeValue((int32_t)currentSigValue);
	}
}

void SigGen::start(uint8_t numCycles)
{
	currentSigValue = getSample(0);
	currentIndex = 0;
	timeInHand_ms = 0;
	cyclesRemaining = numCycles;
	sigGenState = RUNNING;
	broadcastEvent(SIG_GEN_STARTED, 0);
}

void SigGen::start(void)
{
	start((uint8_t)0);
}

void SigGen::start(ISigInput * output)
{
	configOutput(output);
	start((uint8_t)0);
}

void SigGen::start(uint8_t numCycles, ISigInput * output)
{
	configOutput(output);
	start(numCycles);
}

void SigGen::start(uint8_t numCycles, uint16_t startDelay_ms)
{
	start(numCycles);
	timeInHand_ms = startDelay_ms;
	sigGenState = WAITING_TO_START;
}


void SigGen::stop()
{
	sigGenState = STOPPED;
	currentSigValue = 0;
	timeInHand_ms = 0;
	broadcastEvent(SIG_GEN_STOPPED, 0);
	if (!(options & DONT_FINISH_ON_ZERO) && (signalOutput != NULL))
		signalOutput->writeValue(0);

}

bool SigGen::isRunning()
{
	if (sigGenState == SigGen::RUNNING)
		return true;
	return false;
}

int32_t SigGen::readValue()
{
	return currentSigValue;
}

void SigGen::broadcastEvent(uint16_t eventId, uint16_t eventInfo)
{
	EventMngr * em = EventMngr::getMngr();
	em->handleEvent(eventId, eventSourceRef, eventInfo);
}

/*
* gets current and next value and interpolates between them
*/
int32_t SigGen::interpolate()
{
	uint8_t rightShiftRemaining = 16;
	int32_t Yn0, Yn1;
	int32_t interpolatedPart;

	Yn0 = getSample(currentIndex);
	if (currentIndex == numSamples - 1)
		Yn1 = getSample(0);
	else
		Yn1 = getSample(currentIndex + 1);

	interpolatedPart = Cp;
	while (interpolatedPart >= 0x8000)
	{
		interpolatedPart = interpolatedPart >> 1;
		rightShiftRemaining--;
	}

	interpolatedPart *= timeInHand_ms;
	while (interpolatedPart > 0x8000)
	{
		interpolatedPart = interpolatedPart >> 1;
		rightShiftRemaining--;
	}

	interpolatedPart *= (Yn1 - Yn0);
	interpolatedPart = interpolatedPart >> rightShiftRemaining;
	//todo ... avoid overflow
	//	interpolatedPart = (Cp * (Yn1 - Yn0) * timeInHand_ms) >> 16;

	return (Yn0 + interpolatedPart);
}

uint16_t SigGen::getSample(uint8_t sampleNum)
{
	if (options & DONT_USE_PROGMEM)
		return sampleArray[sampleNum];
	else
	    return pgm_read_word_near(sampleArray + sampleNum);
}


