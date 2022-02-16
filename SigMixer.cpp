/*******************************************************************************
SigMixer.cpp Takes a set of signals and sums them together

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
1      02-Dec-2016   Initial release
--------|-------------|--------------------------------------------------------|
1      02-Feb-2017   Updated to use Auto Update
*******************************************************************************/

#include "SigMixer.h"

using namespace SweetMaker;

SigMixer::SigMixer()
{
	output = NULL;
	lastValue = 0;
	thisValue = 0;
}

SigMixer::SigMixer(ISigInput *_output)
{
	output = _output;
	lastValue = 0;
	thisValue = 0;
	isTurnedOn = true;
}

void SigMixer::configOutput(ISigInput *_output)
{
	output = _output;
}

void SigMixer::update(uint16_t elapsedTime_ms)
{
	if ((output == NULL) || (!isTurnedOn))
		return;
	output->writeValue(thisValue);
	lastValue = thisValue;
	thisValue = 0;
}

void SigMixer::writeValue(int32_t inValue)
{
	thisValue += inValue;
}

int32_t SigMixer::readValue()
{
	return (lastValue);
}

void SweetMaker::SigMixer::start()
{
	isTurnedOn = true;
}

void SweetMaker::SigMixer::stop()
{
	isTurnedOn = false;
}
