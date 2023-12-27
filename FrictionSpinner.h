/*******************************************************************************
FrictionSpinner.h - Simulates physics of an object spinning while subject 
                    to friction

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
1      15-May-2019   Initial release
*******************************************************************************/

#ifndef __FRICTION_SLIDER_H__
#define __FRICTION_SLIDER_H__

#include "ISigInput.h"
#include "ISigOutput.h"
#include "Updater.h"

namespace SweetMaker
{
	class FrictionSpinner : public AutoUpdate
	{
		/*
		 * Change in angular velocity = Torque / Inertia
		 */
	public:

		FrictionSpinner();
		void configLimits(int16_t max, int16_t min);

		// Inherited via AutoUpdate
		void update(uint16_t elapsedTime_ms);
		void applyTorque_512(int16_t torque_512);
		void reset();

		int32_t rotation_100000;      // The rotation of the spinner since start
		int32_t angularVel_100000;
		int16_t torque_512;         // Rotational force in NM/256

		int16_t limitMax;
		int16_t limitMin;

		int16_t inertia_512;
		int16_t staticFriction_512; 
		int16_t dynamicFriction_512;

	};
}

#endif