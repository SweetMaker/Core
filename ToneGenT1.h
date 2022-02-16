/*******************************************************************************
ToneGenT1.h Implementation of IToneGen Using AvrTimer1

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

#ifndef __TONE_GEN_H__
#define __TONE_GEN_H__

#include "AvrTimer1.h"
#include "IToneGen.h"

namespace SweetMaker {
	/*
	* ToneGen:
	*/
	class ToneGen_T1_OC_Pins : public IToneGen {
	public:
	    ToneGen_T1_OC_Pins();
	    void setup(void);
	    void setPeriod_us(uint32_t wavelength);
		void setFrequency_hz(uint16_t frequency);
		void writeValue(int32_t period_us);
		void setVolume(uint8_t volume);
		void stop(void);
		private:
		uint8_t volume;
	};


}

#endif