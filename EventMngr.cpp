/*******************************************************************************
EventMngr.h Provides central event distribution point

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
1      30-Jan-2017   Initial release
*******************************************************************************/

#include "EventMngr.h"

using namespace SweetMaker;

#ifndef NULL
#define NULL 0
#endif

static EventMngr * instance = NULL;

EventMngr::EventMngr()
{
	eventHandlerCallback = NULL;
	eventHandlerObject = NULL;
}

EventMngr * EventMngr::getMngr()
{
	if (instance == NULL)
		instance = new EventMngr();
	return instance;
}

void EventMngr::handleEvent(uint16_t eventId, uint8_t srcRef, uint16_t eventInfo)
{
	if (eventHandlerObject != NULL)
		eventHandlerObject->handleEvent(eventId, srcRef, eventInfo);

	if (eventHandlerCallback != NULL)
		eventHandlerCallback(eventId, srcRef, eventInfo);
}

void EventMngr::configCallBack(IEventHandler::EventHandler eventHandler)
{
	eventHandlerCallback = eventHandler;
}

void EventMngr::configCallBack(IEventHandler * eventHandler)
{
	eventHandlerObject = eventHandler;
}
