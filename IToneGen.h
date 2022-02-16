/*******************************************************************************
IToneGen.h Provides Interface for Tone Generator

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

#ifndef __I_TONE_GEN_H__
#define __I_TONE_GEN_H__

#include "ISigInput.h"

namespace SweetMaker {
	/*
	* IToneGen:
	*
	*/
	class IToneGen: public ISigInput {
	public:
		virtual void playPeriod_us(uint32_t period_us, uint16_t duration_ms) = 0;
		virtual void playFrequency_hz(uint16_t frequency_hz, uint16_t duration_ms) = 0;
		virtual void writeValue(int32_t value) = 0;
		virtual void stop(void) = 0;
		virtual void setVolume(uint8_t volume) = 0;
	};

}

#endif