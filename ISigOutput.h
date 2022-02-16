/*******************************************************************************
ISigOutput.h Provides Interface for things which have a Signal as an Output

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

#ifndef __I_SIGOUTPUT_H__
#define __I_SIGOUTPUT_H__

#include <stdint.h>

namespace SweetMaker {
	/*
	* ISigOutput:
	*
	* This has one method for allowing the Signal to be writted
	*/
	class ISigOutput {
	public:
		virtual int32_t readValue() = 0;
	};

	class SigOutput : public ISigOutput {
	public:
		int32_t readValue() {
			return value;
		}
		int32_t value;
	};

	class SigOutput_u8 : public ISigOutput {
	public:
		int32_t readValue() {
			return ((int32_t)value);
		}
		uint8_t value;
	};

}

#endif
