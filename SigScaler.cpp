/*******************************************************************************
SigScaler.cpp Takes a signal and performs a linear transformation and then clips

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
--------|-------------|--------------------------------------------------------|
   2      31-Mar-2017   Added local definitions for uint16_max etc
                        in support of use on OS-X
*******************************************************************************/
#include <limits.h>
#include <Arduino.h>
#include "SigScaler.h"

using namespace SweetMaker;



SigScaler::SigScaler()
{
	output = NULL;
	m = 0;
	c = 0;
	rightShiftQuota = 0;
}

SigScaler::SigScaler(ISigInput * _output)
{
	SigScaler();
	output = _output;
}

SigScaler::SigScaler(int16_t _m, int16_t _c)
{
	SigScaler();
	configLinearScale(m, c);
}

SigScaler::SigScaler(int16_t _m, int16_t _c, ISigInput * output)
{
	SigScaler();
	configLinearScale(m, c);
	configOutput(output);
}


void SigScaler::configOutput(ISigInput * _output)
{
	output = _output;
}

void SigScaler::configLinearScale(int16_t _m_256, int16_t _c)
{
	m = _m_256;
	c = _c;
	rightShiftQuota = 8;

	if (abs(_m_256) >= (int16_max >> 4)) 
	{
		m = m >> 4;
		rightShiftQuota -= 4;
	}
}

void SigScaler::configLinearScale(int32_t fromMin, int32_t fromMax, int32_t toMin, int32_t toMax)
{
	uint8_t shiftQuota = 8;
	int32_t m_256;
	int32_t c;
	
	int32_t toDiff = toMax - toMin;
	int32_t fromDiff = fromMax - fromMin;

	while ((abs(toDiff) < int32_max) && (shiftQuota > 0))
	{
		toDiff *= 2;
		shiftQuota--;
	}

	while ((abs(fromDiff) > uint16_max) && (shiftQuota > 0))
	{
		fromDiff /= 2;
		shiftQuota--;
	}

	m_256 = (toDiff / fromDiff) << shiftQuota;
	
	shiftQuota = 8;
	while ((abs(fromMin) > uint16_max) && (shiftQuota > 0))
	{
		fromMin /= 2;
		shiftQuota--;
	}
	c = toMin - ((m_256 * fromMin) >> shiftQuota);
	configLinearScale((int16_t)m_256, (int16_t)c);
}


void SigScaler::writeValue(int32_t value)
{
	uint8_t shiftQuota = rightShiftQuota;

//	LogMgr::print("In: "); LogMgr::print(value);

	if (abs(value) >= (int32_max >> 4))
	{
		value = value >> 4;
		shiftQuota -= 4;
	}
	value *= m;
	value = value >> shiftQuota;
	value += c;

//	LogMgr::print(" Out: "); LogMgr::println(value);
	if (output == NULL)
		return;

   output->writeValue(value);
}

