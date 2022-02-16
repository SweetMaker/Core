/*******************************************************************************
SigDebug.cpp Takes a signal and performs a linear transformation and then clips

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
1      01-Dec-2016   Initial release
*******************************************************************************/
#include <Arduino.h>
#include "SigDebug.h"


using namespace SweetMaker;

SigDebug::SigDebug()
{
	output = NULL;
	traceOptions = 0;
}

void SigDebug::configOutput(ISigInput * _output)
{
	output = _output;
}

void SigDebug::configTrace(uint32_t options)
{
	traceOptions = options;
}

void SigDebug::writeValue(int32_t value)
{
	if (traceOptions & SerialOut)
	{
		Serial.print(" ");
		Serial.print(value);
		if (traceOptions & NewLine)
		  Serial.println("");
	}

	if(output != NULL)
       output->writeValue(value);
}

