/*******************************************************************************
EepromUtility.cpp - Classes helpful when using EEPROM

Copyright(C) 2016  Howard James May

This program is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.

Contact me at sweet.maker@outlook.com

*******************************************************************************/
#include <Arduino.h>
#ifndef SM_STAND_ALONE
#include <EEPROM.h>
#endif
#include "EepromUtility.h"

using namespace SweetMaker;

class EepromInitialiser
{
public:
	static void initEeprom();
private:
	EepromInitialiser();
};
static EepromInitialiser * globalInitialiser = NULL;


EepromUtility::EepromReader::EepromReader(uint16_t offset, uint16_t len)
{
	bufStart = offset;
	bufLen = len;
	nextByte = offset;

	EepromInitialiser::initEeprom();
}


EepromUtility::EepromWriter::EepromWriter(uint16_t start, uint16_t len)
{
	if (start + len > maxEepromLen)
	{
		return;
	}

	bufStart = start;
	bufLen = len;
	nextByte = start;

	EepromInitialiser::initEeprom();
}




#ifdef ARDUINO_ARCH_AVR

EepromInitialiser::EepromInitialiser()
{
}

void EepromInitialiser::initEeprom()
{
}

EepromUtility::EepromWriter::~EepromWriter()
{
}


uint8_t EepromUtility::EepromReader::readU8()
{
	uint8_t data;
	if (nextByte > bufStart + bufLen)
		return 0;
	data = EEPROM[nextByte++];
	return data;
}

uint16_t EepromUtility::EepromReader::readU16()
{
	uint16_t data;
	if (nextByte + 1 > bufStart + bufLen)
		return 0;
	data = EEPROM[nextByte++];
	data = (data << 8) + EEPROM[nextByte++];
	return data;
}

uint16_t EepromUtility::EepromReader::readU16(uint16_t offset)
{
	uint16_t data;
	if (offset + 1 >= bufLen)
		return 0;
	data = EEPROM[offset++];
	data = (data << 8) + EEPROM[offset];
	return data;
}

uint32_t EepromUtility::EepromReader::readU32()
{
	uint32_t data;
	if (nextByte + 3 > bufStart + bufLen)
		return 0;
	data = EEPROM[nextByte++];
	data = (data << 8) + EEPROM[nextByte++];
	data = (data << 8) + EEPROM[nextByte++];
	data = (data << 8) + EEPROM[nextByte++];
	return data;
}

int8_t EepromUtility::EepromReader::readS8()
{
	int8_t data;
	if (nextByte > bufStart + bufLen)
		return 0;
	data = EEPROM[nextByte++];
	return data;
}

int16_t EepromUtility::EepromReader::readS16()
{
	int16_t data;
	if (nextByte + 1 > bufStart + bufLen)
		return 0;
	data = EEPROM[nextByte++];
	data = (data << 8) + EEPROM[nextByte++];
	return data;
}

int16_t EepromUtility::EepromReader::readS16(uint16_t offset)
{
	int16_t data;
	if (offset + 1 >= bufLen)
		return 0;
	data = EEPROM[offset++];
	data = (data << 8) + EEPROM[offset];
	return data;
}

int32_t EepromUtility::EepromReader::readS32()
{
	int32_t data;
	if (nextByte + 3 > bufStart + bufLen)
		return 0;
	data = EEPROM[nextByte++];
	data = (data << 8) + EEPROM[nextByte++];
	data = (data << 8) + EEPROM[nextByte++];
	data = (data << 8) + EEPROM[nextByte++];
	return data;
}



int EepromUtility::EepromWriter::writeU8(uint8_t data)
{
	if (nextByte >= bufStart + bufLen)
		return -1;
	EEPROM[nextByte++] = data;
	return (0);
}

int EepromUtility::EepromWriter::writeU16(uint16_t data)
{
	if (nextByte + 1 >= bufStart + bufLen)
		return -1;
	EEPROM[nextByte++] = data >> 8;
	EEPROM[nextByte++] = data & 0xff;
	return (0);
}

int EepromUtility::EepromWriter::writeU32(uint32_t data)
{
	if (nextByte + 3 >= bufStart + bufLen)
		return -1;
	EEPROM[nextByte++] = data >> 24;
	EEPROM[nextByte++] = (data >> 16) & 0xff;
	EEPROM[nextByte++] = (data >> 8) & 0xff;
	EEPROM[nextByte++] = data & 0xff;
	return (0);
}


int EepromUtility::EepromWriter::writeS8(int8_t data)
{
	if (nextByte >= bufStart + bufLen)
		return -1;
	EEPROM[nextByte++] = data;
	return (0);
}

int EepromUtility::EepromWriter::writeS16(int16_t data)
{
	if (nextByte + 1 >= bufStart + bufLen)
		return -1;
	EEPROM[nextByte++] = data >> 8;
	EEPROM[nextByte++] = data & 0xff;
	return (0);
}

int EepromUtility::EepromWriter::writeS32(int32_t data)
{
	if (nextByte + 3 >= bufStart + bufLen)
		return -1;
	EEPROM[nextByte++] = data >> 24;
	EEPROM[nextByte++] = (data >> 16) & 0xff;
	EEPROM[nextByte++] = (data >> 8) & 0xff;
	EEPROM[nextByte++] = data & 0xff;
	return (0);
}

#endif

#ifdef ARDUINO_ARCH_ESP32

EepromInitialiser::EepromInitialiser()
{
	EEPROM.begin(0x400);
}

void EepromInitialiser::initEeprom()
{
	if (globalInitialiser == NULL)
		globalInitialiser = new EepromInitialiser();
}

EepromUtility::EepromWriter::~EepromWriter()
{
	EEPROM.commit();
}

uint8_t EepromUtility::EepromReader::readU8()
{
	uint8_t data;
	if (nextByte > bufStart + bufLen)
		return 0;
	data = EEPROM.read(nextByte++);
	return data;
}

uint16_t EepromUtility::EepromReader::readU16()
{
	uint16_t data;
	if (nextByte + 1 > bufStart + bufLen)
		return 0;
	data = EEPROM.read(nextByte++);
	data = (data << 8) + EEPROM.read(nextByte++);
	return data;
}

uint16_t EepromUtility::EepromReader::readU16(uint16_t offset)
{
	uint16_t data;
	if (offset + 1 >= bufLen)
		return 0;
	data = EEPROM.read(offset++);
	data = (data << 8) + EEPROM.read(offset);
	return data;
}

uint32_t EepromUtility::EepromReader::readU32()
{
	uint32_t data;
	if (nextByte + 3 > bufStart + bufLen)
		return 0;
	data = EEPROM.read(nextByte++);
	data = (data << 8) + EEPROM.read(nextByte++);
	data = (data << 8) + EEPROM.read(nextByte++);
	data = (data << 8) + EEPROM.read(nextByte++);
	return data;
}

int8_t EepromUtility::EepromReader::readS8()
{
	int8_t data;
	if (nextByte > bufStart + bufLen)
		return 0;
	data = EEPROM.read(nextByte++);
	return data;
}

int16_t EepromUtility::EepromReader::readS16()
{
	int16_t data;
	if (nextByte + 1 > bufStart + bufLen)
		return 0;
	data = EEPROM.read(nextByte++);
	data = (data << 8) + EEPROM.read(nextByte++);
	return data;
}

int16_t EepromUtility::EepromReader::readS16(uint16_t offset)
{
	int16_t data;
	if (offset + 1 >= bufLen)
		return 0;
	data = EEPROM.read(offset++);
	data = (data << 8) + EEPROM.read(offset);
	return data;
}

int32_t EepromUtility::EepromReader::readS32()
{
	int32_t data;
	if (nextByte + 3 > bufStart + bufLen)
		return 0;
	data = EEPROM.read(nextByte++);
	data = (data << 8) + EEPROM.read(nextByte++);
	data = (data << 8) + EEPROM.read(nextByte++);
	data = (data << 8) + EEPROM.read(nextByte++);
	return data;
}



int EepromUtility::EepromWriter::writeU8(uint8_t data)
{
	if (nextByte >= bufStart + bufLen)
		return -1;
	EEPROM.write(nextByte++, data);
	return (0);
}

int EepromUtility::EepromWriter::writeU16(uint16_t data)
{
	if (nextByte + 1 >= bufStart + bufLen)
		return -1;
	EEPROM.write(nextByte++, data >> 8);
	EEPROM.write(nextByte++, data & 0xff);
	return (0);
}

int EepromUtility::EepromWriter::writeU32(uint32_t data)
{
	if (nextByte + 3 >= bufStart + bufLen)
		return -1;
	EEPROM.write(nextByte++, data >> 24);
	EEPROM.write(nextByte++, (data >> 16) & 0xff);
	EEPROM.write(nextByte++, (data >> 8) & 0xff);
	EEPROM.write(nextByte++, data & 0xff);
	return (0);
}


int EepromUtility::EepromWriter::writeS8(int8_t data)
{
	if (nextByte >= bufStart + bufLen)
		return -1;
	EEPROM.write(nextByte++, data);
	return (0);
}

int EepromUtility::EepromWriter::writeS16(int16_t data)
{
	if (nextByte + 1 >= bufStart + bufLen)
		return -1;
	EEPROM.write(nextByte++, data >> 8);
	EEPROM.write(nextByte++, data & 0xff);
	return (0);
}

int EepromUtility::EepromWriter::writeS32(int32_t data)
{
	if (nextByte + 3 >= bufStart + bufLen)
		return -1;
	EEPROM.write(nextByte++, data >> 24);
	EEPROM.write(nextByte++, (data >> 16) & 0xff);
	EEPROM.write(nextByte++, (data >> 8) & 0xff);
	EEPROM.write(nextByte++, data & 0xff);
	return (0);
}

#endif






/*
* The width of the CRC calculation and result.
* Modify the typedef for a 16 or 32-bit CRC standard.
*/
uint16_t EepromUtility::calculateCrc(uint16_t const start, uint16_t const nBytes)
{
	typedef uint16_t crcValue;
	static const uint16_t crcValueWidth = (8 * sizeof(crcValue));
	static const uint16_t topBit = (1 << (crcValueWidth - 1));
	static const uint16_t polynomial = 0xD8;  /* 11011 followed by 0's */

	crcValue  remainder = 0;

	EepromReader reader(start, nBytes);

	/*
	* Perform modulo-2 division, a byte at a time.
	*/
	for (uint16_t byte = 0; byte < nBytes; ++byte)
	{
		/*
		* Bring the next byte into the remainder.
		*/
		remainder ^= (reader.readU8() << (crcValueWidth - 8));

		/*
		* Perform modulo-2 division, a bit at a time.
		*/
		for (uint8_t bit = 8; bit > 0; --bit)
		{
			/*
			* Try to divide the current data bit.
			*/
			if (remainder & topBit)
			{
				remainder = (remainder << 1) ^ polynomial;
			}
			else
			{
				remainder = (remainder << 1);
			}
		}
	}

	return (remainder);
}