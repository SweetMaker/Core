/*******************************************************************************
ToDiscrete.h Takes a continous signal and converts it into discrete steps with hysteresis

Copyright(C) 2023  Howard James May

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
   1      02-Feb-2023   Initial release
*******************************************************************************/

#ifndef __TO_DISCRETE_H__
#define __TO_DISCRETE_H__

#ifndef NULL
#define NULL (0)
#endif

#include "IEventHandler.h"
#include "ISigInput.h"
#include "ISigOutput.h"
#include "Updater.h"

namespace SweetMaker
{
	class ToDiscrete : public ISigInput
	{
	public:
		ToDiscrete();
		ToDiscrete(uint16_t step_size, uint16_t hysteresis_size);
		ToDiscrete(uint16_t step_size, uint16_t hysteresis_size, uint8_t id);

		void writeValue(int32_t value);
		int32_t readValue();

		void start(int32_t current_value);
		void stop();

		int16_t current_discrete_value;
		int32_t current_continuous_value;
		int32_t in_step_value;
		uint8_t distance_to_mid_255;  // distance to midpoint scaled to 255

		typedef enum
		{
			NEW_VALUE = IEventHandler::TO_DISCRETE_EVENTS + 0
		} TO_DISCRETE_EVENT;

		uint16_t calculateDistanceToMid();
		uint16_t distance_to_mid_scaler;

	private:
		bool started;
		uint8_t instance_id;
		uint16_t hysteresis_size;
		uint16_t step_size;
		int32_t next_up_threshold;
		int32_t next_down_threshold;
		ISigInput* output;

		void print();
	};

}

#endif