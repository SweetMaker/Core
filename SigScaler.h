/*******************************************************************************
SigScaler.h Takes a signal and performs a linear transformation and then clips

Copyright(C) 2016-2017  Howard James May

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


#ifndef __SIG_SCALER_H__
#define __SIG_SCALER_H__

#ifndef NULL
#define NULL (0)
#endif

#include "ISigInput.h"

namespace SweetMaker
{
	class SigScaler :
		public ISigInput
	{
	public:
		SigScaler();
		SigScaler(ISigInput * output);
		SigScaler(int16_t m_256, int16_t c);
		SigScaler(int16_t m_256, int16_t c, ISigInput * output);

		void configOutput(ISigInput * output);
		void configLinearScale(int16_t m_256, int16_t c);
		void configLinearScale(int32_t fromMin, int32_t fromMax, int32_t toMin, int32_t toMax);

		void writeValue(int32_t value);

	private:
		ISigInput * output;
		int32_t m;
		int16_t c;
		uint8_t rightShiftQuota;

		const static uint32_t int16_max = 32767;
		const static uint32_t int32_max = 2147483647;
		const static uint32_t uint16_max = 0xffff;

	};
}

#endif