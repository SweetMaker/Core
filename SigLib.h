/*******************************************************************************
SigLib.h Takes data samples; provides time varying signal.

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


#ifndef __SIG_LIB_H__
#define __SIG_LIB_H__

#include "SigGen.h"

namespace SweetMaker
{
	static const SigGen::SAMPLE PROGMEM clippedTriangleWave[] = { 32, 64, 128, 255, 255, 128, 64, 32 };
	static const SigGen::SAMPLE PROGMEM triangleWave255[] = { 0, 255 };
	static const SigGen::SAMPLE PROGMEM hbtWave[] = { 51, 51, 47, 47, 47, 42, 42, 42, 29, 21, 0, 21, 21, 47, 55, 47, 0, 29, 36, 51, 55, 47, 47, 47, 42, 36, 59, 42, 104, 186, 255, 174, 0, 132, 157, 123, 86, 66, 62, 55, 47, 36, 0, 0, 29, 42, 51, 55, 59, 72, 72, 78, 88, 98, 106, 108, 110, 93, 81, 29, 47, 66, 72, 75, 72, 72, 72, 69, 62, 59, 55, 51, 51, 51, 51, 47, 47, 47, 47, 51 };
	static const SigGen::SAMPLE PROGMEM breathingWave[] = { 0, 180, 255, 255, 220, 200, 160, 120, 80, 40 };
}

#endif