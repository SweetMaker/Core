/*******************************************************************************
ToneGenT0.cpp Various Implementations of IToneGen

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

#ifdef ARDUINO_AVR_NANO

#include <Arduino.h>
#include "ToneGenT0.h"

using namespace SweetMaker;

ToneGenT0::ToneGenT0()
{

}
void ToneGenT0::setup(uint8_t pin)
{
	tonePin = pin;
}

void ToneGenT0::playPeriod_us(uint32_t period_us, uint16_t duration_ms)
{
	uint32_t freq_hz = 0;
	if(period_us != 0)
	  freq_hz = 1000000 / period_us;
	playFrequency_hz(freq_hz, duration_ms);
}

void ToneGenT0::playFrequency_hz(uint16_t frequency, uint16_t duration_ms)
{
	::tone(tonePin, frequency, duration_ms);
}

void ToneGenT0::writeValue(int32_t period_us)
{
	playPeriod_us(period_us, 0);
}


void ToneGenT0::setVolume(uint8_t volume)
{

}

void ToneGenT0::stop(void)
{
	::noTone(tonePin);
}

#endif
