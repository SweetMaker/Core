/*******************************************************************************
 AsyncAdc.cpp: Class for interfacing to Avr328p Timer1

 Copyright(C) 2018  Howard James May

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
   1      09-Oct-2018   Initial release
*******************************************************************************/
#ifdef NEVER_DEFINED

#include <stdlib.h>
#include <string.h>
#include <Arduino.h>
#include "AvrAdc.h"
#include "AsyncAdc.h"


using namespace SweetMaker;

AsyncAdcMngr::AsyncAdcMngr(uint8_t numChannels)
{
	int memSize = sizeof(ASYN_ADC_CHANNEL) * numChannels;
	this->channels = (ASYN_ADC_CHANNEL *) malloc(memSize);
	if (this->channels == 0)
	{
		Serial.println("AsyncAdcMngr initialisation failed");
		exit(-1);
	}
	memset(channels, 0, memSize);
	this->maxChannels = numChannels;
	this->currentChannel = -1; // This should be the case when it is not running
}

int AsyncAdcMngr::initAsyncAdcMngr(uint8_t numChannels)
{
	int memSize = sizeof(ASYN_ADC_CHANNEL) * numChannels;
	AsyncAdcMngr::channels = (ASYN_ADC_CHANNEL *)malloc(memSize);
	if (AsyncAdcMngr::channels == 0)
	{
		Serial.println("AsyncAdcMngr initialisation failed");
		exit(-1);
	}
	memset(channels, 0, memSize);
	AsyncAdcMngr::maxChannels = numChannels;
	AsyncAdcMngr::currentChannel = -1; // This should be the case when it is not running
	return (0);
}

/*
* This spins through all channels checking for a free channel and also 
* for whether the channel is already active and also whether there are
* already enabled channels.
*/
int AsyncAdcMngr::enablePin(uint8_t pinNum)
{
	ASYN_ADC_CHANNEL * channel;
	int channelId;
	int newChannelId = -1;
	bool isAlreadyActive = false;

	for (channelId = 0; channelId < maxChannels; channelId++)
	{
		channel = &channels[channelId];
		if (channel->state == DISABLED)
		{
			/*
			 * We've found a free channel which we can use if
			 */
			if(newChannelId == -1)
    			newChannelId = channelId;
		}
		else
		{
			if (channel->pinNum == pinNum)
				return (PIN_ALREADY_ENABLED);
			isAlreadyActive = true;
		}
	}
    
	if(newChannelId < 0)
	    return (INSUFFICIENT_CHANNELS);

	if (!isAlreadyActive)
		startAsyncReading();

	channels[newChannelId].state = ENABLED_STALE;
	channels[newChannelId].pinNum = pinNum;
	channels[newChannelId].precision = AvrAdc::ADCP_MEDIUM_U10;
	return (0);
}

/*
* interruptHandler - this takes a reading for the current channel (if active)
*                    and then starts a reading for the next channel. 
*/
void AsyncAdcMngr::interruptHandler(void)
{
	ASYN_ADC_CHANNEL *channel;
	int8_t nextChannelId;

	if ((currentChannel != -1) &&
		(currentChannel < maxChannels))
	{
		channel = channels + currentChannel;
		if (channel->state != DISABLED)
			channel->lastReading = AvrAdc::getResult_u16();
	}
	nextChannelId = getNextChannel();
	if (nextChannelId == -1)
	{
		stopAsyncReading();
	}
	channel = channels + nextChannelId;
	AvrAdc::startConversion(channel->pinNum);
	currentChannel = nextChannelId;
}

int AsyncAdcMngr::startAsyncReading()
{
	int8_t firstChannelId;
	AvrAdc::enableAdc();
	AvrAdc::setPrecision(AvrAdc::ADCP_MEDIUM_U10);
	firstChannelId = getNextChannel();
	if (firstChannelId != -1)
	{
		AvrAdc::startConversion(channels[firstChannelId].pinNum);
		currentChannel = firstChannelId;
	}
	return 0;
}

int AsyncAdcMngr::stopAsyncReading()
{
	return 0;
}

/*
* getNextChannel - returns the next channel to read
*                  This works even if there is only one channel
*                  This works even if the last channel is NULL(-1)
*
* returns channel ID of next channel
*         -1 if there are no active channels
*/
int8_t AsyncAdcMngr::getNextChannel()
{
	uint8_t i;
	uint8_t id = (uint8_t)(currentChannel + 1);

	for (i = 0; i < maxChannels; i++)
	{
		if (id >= maxChannels)
		{
			id = 0;
			if (channels[id].state != DISABLED)
				return (id);
		}
	}
	return (-1);
}

int AsyncAdcMngr::readPin(uint8_t pinNum)
{
    ASYN_ADC_CHANNEL *channel = &channels[0];
	for (int channelId = 0; channelId < maxChannels; channelId++, channel++)
	{
		if(channel->pinNum == pinNum)
		{
			switch (channel->state)
			{
			default:
			case DISABLED:
				return (0);
			case ENABLED_STALE:
				return (0);
			case ENABLED:
				return (channel->lastReading);
			}
		}
	}
	return 0;
}

int AsyncAdcMngr::disablePin(uint8_t pinNum)
{
	ASYN_ADC_CHANNEL * channel;
	int channelId;
	bool still_active = false;
	bool found_pin = false;

	for (channelId = 0; channelId < maxChannels; channelId++)
	{
		channel = &channels[channelId];
		if (channel->state != DISABLED)
		{
			if (channel->pinNum == pinNum)
			{
				found_pin = true;
				channel->state = DISABLED;
			}
			else
			{
				still_active = true;
			}
		}
	}

	if (!still_active)
	{
		stopAsyncReading();
	}

	if (!found_pin)
		return (-1);
	return (0);
}

ISR(ADC_vect)
{
	AsyncAdcMngr::interruptHandler();
}

#endif