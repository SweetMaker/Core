/*******************************************************************************
StaticGen.cpp - Generates a Static Signal

Copyright(C) 2016  Howard James May

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
1      03-Dec-2016   Initial release
*******************************************************************************/

#include <Arduino.h>
#include "StaticGen.h"

using namespace SweetMaker;

StaticGen::StaticGen()
{
	initialise();
}

StaticGen::StaticGen(ISigInput * output)
{
	initialise();
	configOutput(output);
}

void StaticGen::initialise()
{
	output = NULL;
	staticSignal = 0;
	remainingStaticEventTime_ms = 0;
	duty_256 = defaultDuty_256;
	staticEventPeriod_ms = defaultStaticPeriod_ms;
}

void StaticGen::configOutput(ISigInput * output)
{
	this->output = output;
}

void StaticGen::configPeriod_ms(uint8_t period)
{
	if (period == 0)
		period = 1;
	staticEventPeriod_ms = period;
}

void StaticGen::configDuty_256(uint8_t duty)
{
	duty_256 = duty;
}

void StaticGen::update(uint16_t elapsedTime_ms)
{
	if (remainingStaticEventTime_ms <= elapsedTime_ms)
	{
		if (random(0, 255) < duty_256)
		{
			staticSignal = 255;
		}
		else
		{
			staticSignal = 0;
		}
		remainingStaticEventTime_ms = defaultStaticPeriod_ms;
	}
	else
		remainingStaticEventTime_ms -= elapsedTime_ms;

	if (output != NULL)
		output->writeValue(staticSignal);
}

int32_t StaticGen::readValue()
{
	return (staticSignal);
}

