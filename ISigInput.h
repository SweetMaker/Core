/*******************************************************************************
ISigInput.h Provides Interface for things which take a Signal as an Input

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
   1      28-Nov-2016   Initial release
*******************************************************************************/

#ifndef __I_SIGINPUT_H__
#define __I_SIGINPUT_H__

#include <stdint.h>

namespace SweetMaker {
	/*
	* ISigInput:
	*
	* This has one method for allowing the Signal to be written
	*/
	class ISigInput {
	public:
		virtual void writeValue(int32_t value) = 0;
	};

	class SigInput : public ISigInput {
	public:
		void writeValue(int32_t _value) {
			value = _value;
		}
		int32_t value;
	};

	class SigInput_u8 : public ISigInput {
	public:
		void writeValue(int32_t _value) {
			value = (uint8_t)_value;
		}
		uint8_t value;
	};

	class SigInput_u8p : public ISigInput {
	public:
		SigInput_u8p(uint8_t *value) {
			this->value = value;
		}

		void writeValue(int32_t _value) {
			*value = (uint8_t)_value;
		}

		uint8_t * value;
	};

	class SigInput_bool : public ISigInput {
	public:
		SigInput_bool()
		{
			value = false;
		}

		void writeValue(int32_t _value) {
			if (_value == 0)
				value = false;
			else
				value = true;
		}

		bool value;
	};

}

#endif
