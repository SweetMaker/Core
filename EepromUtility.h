/*******************************************************************************
EepromUtility.h Helper functions for using EEPROM 

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
   1      26-Oct-2016   Initial release
*******************************************************************************/
#ifndef __EEPROM_UTILITY_H__
#define __EEPROM_UTILITY_H__

#include <stdint.h>


namespace SweetMaker
{
	class EepromUtility
	{

	public:
		EepromUtility();

		class EepromReader {
		public:
			EepromReader(uint16_t offset, uint16_t len);
			uint8_t readU8(void);
			uint8_t readU8(uint16_t offset);
			uint16_t readU16(void);
			uint16_t readU16(uint16_t offset);
			uint32_t readU32(void);
			uint32_t readU32(uint32_t offset);

			int8_t readS8(void);
			int8_t readS8(uint16_t offset);
			int16_t readS16(void);
			int16_t readS16(uint16_t offset);
			int32_t readS32(void);
			int32_t readS32(uint32_t offset);
		private:
			uint16_t bufStart;
			uint16_t bufLen;
			uint16_t nextByte;
		};

		class EepromWriter {
		public:
			EepromWriter(uint16_t offset, uint16_t len);
			~EepromWriter();
			int writeU8(uint8_t data);
			int writeU16(uint16_t data);
			int writeU32(uint32_t data);

			int writeS8(int8_t data);
			int writeS16(int16_t data);
			int writeS32(int32_t data);
		private:
			uint16_t bufStart;
			uint16_t bufLen;
			uint16_t nextByte;
		};

		static uint16_t calculateCrc(uint16_t const start, uint16_t const nBytes);
		static const uint16_t maxEepromLen=1024;
	};
};

#endif