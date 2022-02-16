/*******************************************************************************
PerfMon.cpp - Measures CPU and Memory resource utilisation

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

#include "Arduino.h"
#include "PerfMon.h"

using namespace SweetMaker;

PerfMon * singleInstance = NULL;

PerfMon * PerfMon::getPerfMon()
{
	if(singleInstance == NULL)
	{
		singleInstance = new PerfMon();
	}
	return singleInstance;
}

PerfMon::PerfMon()
{
	reset();
}

void PerfMon::reset()
{
	intervalCount = 0;
	maxInterval = 0;
	minInterval = 0xffff;
	intervalSum = 0;
	intervalStartTime_us = micros();

	for (int i = 0; i < PERF_MON_MAX_DIST_SEGMENTS; i++)
		intervalDistribution[i] = 0;
}

void PerfMon::intervalStart()
{
	intervalStartTime_us = micros();
}

void PerfMon::intervalStop()
{
	unsigned long interval_us = micros() - intervalStartTime_us;
	if(interval_us > maxInterval)
		maxInterval = interval_us;
	if(interval_us < minInterval)
		minInterval = interval_us;
	intervalSum += interval_us;
	intervalCount ++;

	int i = 0;
	while(interval_us != 0)
	{
		interval_us = interval_us >> 1;
		i++;
	}
	if(i < PERF_MON_MAX_DIST_SEGMENTS)
		intervalDistribution[i]++;
}

unsigned long PerfMon::averageInterval()
{
	if(intervalCount == 0)
		return 0;
	return (intervalSum / intervalCount);
}


uint32_t PerfMon::freeRam ()
{
	#ifdef ARDUINO_ARCH_AVR
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
	#else
	return (0);
	#endif
}

void PerfMon::print()
{
	unsigned long i, max, min;
	Serial.print(F("PerfMon Results: LoopCount: "));
	Serial.println(intervalCount);
	Serial.print(F("Free Mem: "));
	Serial.println(freeRam());
	Serial.print(F("Max Int: "));
	Serial.println(maxInterval);
	Serial.print(F("Min Int: "));
	Serial.println(minInterval);
	Serial.print(F("Avg: "));
	Serial.println(averageInterval());
	min = 1;
	max = 2;
	for(i = 1; i < PERF_MON_MAX_DIST_SEGMENTS; i++)
	{
		Serial.print(min);
		Serial.print(F("\t to \t"));
		Serial.print(max);
		Serial.print(F("\t:"));
		Serial.println(intervalDistribution[i]);
		min *= 2;
		max *= 2;
	}
}
