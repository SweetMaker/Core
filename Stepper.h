/*******************************************************************************
Stepper.h - Takes a 'continuous' signal and converts it into discrete steps

Copyright(C) 2019  Howard James May

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
1      19-Mar-2019   Initial release
*******************************************************************************/

#ifndef __STEPPER_H__
#define __STEPPER_H__

#include "ISigInput.h"
#include "ISigOutput.h"
#include "Updater.h"

namespace SweetMaker
{
	class Stepper : public ISigInput
	{
	public:

		typedef enum {
			UNLIMITED_STEPS = 0x0001
		}OPTIONS;

		Stepper();
		Stepper(int16_t min, int16_t max, uint16_t num_steps);

		void configSteps(int16_t min, int16_t max, uint16_t num_steps);
		void configOptions(OPTIONS options);
		bool update(int16_t reading);
		void writeValue(int32_t value);

		int16_t currentStep;

//	private:
		int16_t rangeMin;
		int16_t rangeSize;
		int16_t Cm; // Multiplication factor then divide by 2^16;
		int16_t hysteresis;
		uint16_t numSteps;

		int16_t calculateStep(int16_t input);
		uint8_t options = 0;
	};
}

#endif