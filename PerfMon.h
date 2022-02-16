/*******************************************************************************
PerfMon.h - Measures CPU and Memory resource utilisation

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


#ifndef __PERF_MON_H__
#define __PERF_MON_H__

#include <stdint.h>

extern int __heap_start, *__brkval;

namespace SweetMaker
{
#define PERF_MON_MAX_DIST_SEGMENTS (16)

class PerfMon
{
public:

	static PerfMon * getPerfMon();
	void reset();
	void intervalStart();
	void intervalStop();
	unsigned long averageInterval();

	static uint32_t freeRam();
	void print();

private:
	PerfMon();

	unsigned long intervalStartTime_us;

	uint32_t intervalCount;
	unsigned long intervalSum;

	unsigned long maxInterval;
	unsigned long minInterval;
	uint16_t intervalDistribution[PERF_MON_MAX_DIST_SEGMENTS];
};
}
#endif



