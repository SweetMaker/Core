/*******************************************************************************
Updater.cpp

Copyright(C) 2017  Howard James May

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
    1     31-Jan-2017   Initial release
    2     17-Apr-2017   Removed unused update frequency
                        Renamed to AutoUpdater
    3     23-May-2019   Added update method with no elapsedTime parameter
*******************************************************************************/

#include <Arduino.h>
#include "Updater.h"

using namespace SweetMaker;

#ifndef NULL
#define NULL 0
#endif

AutoUpdateMngr * AutoUpdateMngr::getUpdater()
{
	static AutoUpdateMngr * instance = NULL;
	if (instance == NULL)
		instance = new AutoUpdateMngr();
	return instance;
}

AutoUpdateMngr::AutoUpdateMngr()
{
	this->updatableListHead = NULL;
	this->lastUpdateTime_ms = millis();
}

void AutoUpdateMngr::update()
{
	unsigned long thisTime_ms = millis();
	uint16_t elapsedTime_ms = (uint16_t)(thisTime_ms - lastUpdateTime_ms);
	lastUpdateTime_ms = thisTime_ms;
	update(elapsedTime_ms);
}

void AutoUpdateMngr::update(uint16_t elapsedTime_ms)
{
	AutoUpdate * updatableIter = this->updatableListHead;

	while (updatableIter != NULL)
	{
		/*
		 * We need to capture the nextAutoUpdate here as the current one
		 * may be removed from the AutoUpdater during it's update process
		 * thus losing our ability to find the next.
		 */
		AutoUpdate * nextAutoUpdate = updatableIter->nextUpdatable;
		updatableIter->update(elapsedTime_ms);
		updatableIter = nextAutoUpdate;
	}
}

AutoUpdate::AutoUpdate()
{
	addToUpdater();
}

void AutoUpdate::addToUpdater()
{
	AutoUpdateMngr * updater = AutoUpdateMngr::getUpdater();
	this->nextUpdatable = updater->updatableListHead;
	updater->updatableListHead = this;
}

void AutoUpdate::removeFromUpdater()
{
	AutoUpdateMngr * updater = AutoUpdateMngr::getUpdater();

	if (updater->updatableListHead == NULL)
		return;

	if (updater->updatableListHead == this)
	{
		updater->updatableListHead = updater->updatableListHead->nextUpdatable;
		this->nextUpdatable = NULL;
		return;
	}

	AutoUpdate *iter = updater->updatableListHead;
	while (iter->nextUpdatable != NULL)
	{
		if (iter->nextUpdatable == this)
		{
			iter->nextUpdatable = this->nextUpdatable;
			return;
		}
		iter = iter->nextUpdatable;
	}

	return;
}


AutoUpdate::~AutoUpdate()
{
	removeFromUpdater();
}

