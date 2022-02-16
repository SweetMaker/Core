/*******************************************************************************
SigGen.h - Generates a Static Signal

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

#ifndef __STATIC_GEN_H__
#define __STATIC_GEN_H__

#include "ISigInput.h"
#include "ISigOutput.h"
#include "Updater.h"

namespace SweetMaker
{
	class StaticGen : public ISigOutput, private AutoUpdate
	{
	public:
		StaticGen();
		StaticGen(ISigInput * output);
		void configOutput(ISigInput * output);
		void configDuty_256(uint8_t duty);
		void configPeriod_ms(uint8_t period);

		int32_t readValue();

	protected:
		void update(uint16_t elapsedTime_ms);

	private:
		void initialise();
		ISigInput * output;
		static const uint8_t defaultStaticPeriod_ms = 5;
		static const uint8_t defaultDuty_256 = 25;

		uint8_t staticEventPeriod_ms;
		uint8_t duty_256;

		uint8_t remainingStaticEventTime_ms = 0;
		uint8_t staticSignal;
	};
}

#endif