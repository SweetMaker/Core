/*******************************************************************************
SigGen.h Takes data samples; provides time varying signal.

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
*******************************************************************************/


#ifndef __SIG_GEN_H__
#define __SIG_GEN_H__

#ifndef NULL
#define NULL (0)
#endif

#include "ISigOutput.h"
#include "ISigInput.h"
#include "IEventHandler.h"
#include "Updater.h"

namespace SweetMaker
{

/*
* SigGen: Geneates a time dependent Wave form from a set of samples
*
* This takes an array of data points representing samples for the required waveform and the period
* of the waveform in ms. In addition a scale factor is used to allow the signal output to be amplified
* or attenuated.
*
* The Signal Generator is updated by the AutoUpdater which causes the signal output
* to be updated to the correct value. This value will by default interpolate between the neaest two values.
* Interpolation can be disabled.
*
*/
class SigGen : public ISigOutput, private AutoUpdate
{
public:
	typedef uint16_t SAMPLE;

#define SAMPLE_SIZE (sizeof(SigGen::SAMPLE))
#define NUM_SAM(s) (sizeof(s)>>1)

	SigGen();
	SigGen(const SAMPLE * samples, uint8_t numSamples, uint32_t period_ms, uint8_t options);

	void configSamples(const SAMPLE * samples, uint8_t numSamples, uint32_t period_ms, uint8_t options);
	void configOutput(ISigInput * signalUser);
	void configPeriod_ms(uint32_t period_ms);
	void configEventRef(uint8_t sourceReference);
	void configOptions(uint8_t options);

	void start();
	void start(uint8_t numCycles);
	void start(ISigInput * output);
	void start(uint8_t numCycles, ISigInput * output);
	void start(uint8_t numCycles, uint16_t startDelay_ms);

	void stop();

	int32_t readValue();
	bool isRunning();

	typedef enum
	{
	    SIG_GEN_STOPPED = IEventHandler::SIG_GEN_EVENTS + 0,
		SIG_GEN_STARTED = IEventHandler::SIG_GEN_EVENTS + 1,
		SIG_GEN_FINISHED = IEventHandler::SIG_GEN_EVENTS + 2
	} SIG_GEN_EVENT;

	typedef enum
	{
	    INTERPOLATE_OFF = 0x01,
		DONT_USE_PROGMEM = 0x02,
		DONT_FINISH_ON_ZERO = 0x04
	} SIG_GEN_OPTIONS;

	void update(uint16_t elapsedTime_ms);

private:

	typedef enum
	{
		UNCONFIGURED = 0,
		STOPPED = 1,
		WAITING_TO_START = 2,
		RUNNING = 3,
	} SIG_GEN_STATE;

	uint8_t sigGenState;

	ISigInput * signalOutput;
	uint8_t eventSourceRef;
	const SAMPLE * sampleArray;
	uint8_t numSamples;
	uint16_t samplePeriod_ms;
	uint8_t options;
	
	uint8_t cyclesRemaining;
	uint8_t currentIndex;
	uint16_t timeInHand_ms;

	uint16_t currentSigValue;

	/*
	* calculate current and next value and interpolates between them
	*/
	uint32_t Cp; // Value calculated when period is set.

	int32_t interpolate();
	void initialise();
	void broadcastEvent(uint16_t eventId, uint16_t eventInfo);
	uint16_t getSample(uint8_t sampleNum);
};

extern const SigGen::SAMPLE sineWave255[32] ;

}

#endif