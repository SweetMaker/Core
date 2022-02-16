/*******************************************************************************
LedDriver.cpp

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
   2      27-Nov-2016   Stripped out SigGen and introduced IReadable method
*******************************************************************************/

#include "PiezoDriver.h"
#include "LedDriver.h"
#include "Platform.h"

using namespace SweetMaker;

LedDriver::LedDriver()
{
	balanceLevel = 255;
}
void LedDriver::configPin(uint8_t pinNum)
{
	ledPin = pinNum;
	pinMode(ledPin, OUTPUT);
	digitalWrite(ledPin, LOW);
}

void LedDriver::configLevelling(uint8_t _level)
{
	balanceLevel = _level;
}

void LedDriver::turnOn()
{
	calculateAndSetOutput(255);
}

void LedDriver::turnOn(uint8_t brightness)
{
	calculateAndSetOutput(brightness);
}

void LedDriver::writeValue(int32_t value)
{
	if (value > 255)
		value = 255;
	else if (value < 0)
		value = 0;

	turnOn((uint8_t)value);
}

void LedDriver::turnOff()
{
	digitalWrite(ledPin, LOW);
}

void LedDriver::calculateAndSetOutput(uint8_t brightness)
{
	uint16_t analogOutValue = brightness;
	if (balanceLevel < 255)
		analogOutValue = (brightness * (balanceLevel + 1)) >> 8;
	analogOutValue = perceivedBrightnessCorrection(analogOutValue);
	analogWrite(ledPin, analogOutValue);
}


const uint8_t PROGMEM correctionTable[] =
{
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
	1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
	2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
	5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
	10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
	17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
	25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
	37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
	51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
	69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
	90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
	115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
	144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
	177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
	215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

uint8_t LedDriver::perceivedBrightnessCorrection(uint8_t requestedBrightness)
{
	return pgm_read_byte(&correctionTable[requestedBrightness]);
}



