/*******************************************************************************
LedStripDriver.h

This is an example of how simple driving a Neopixel can be
This code is optimized for understandability and changability rather than raw speed
More info at http://wp.josh.com/2014/05/11/ws2812-neopixels-made-easy/



These values depend on which pin your string is connected to and what board you are using 
More info on how to find these at http://www.arduino.cc/en/Reference/PortManipulation



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
1      02-May-2019   Initial release
*******************************************************************************/


#ifndef __LED_STRIP_DRIVER_H__
#define __LED_STRIP_DRIVER_H__

#include <stdint.h>
#include "IEventHandler.h"
#include "Colour.h"


namespace SweetMaker {

	class ILedStripDriver
	{

	public:
		virtual int setNextLed(ColourRGB *) = 0;
		virtual int show() = 0;
		virtual int show(ColourRGB *, uint16_t numLed) = 0;
		virtual uint16_t getRefreshTime_us(uint16_t numLed) = 0;

		enum
		{
			LSE_CONFIG_ERR = IEventHandler::LED_STRIP_EVENTS + 0,
			LSE_SHOW_ERR = IEventHandler::LED_STRIP_EVENTS + 1,
		}LED_STRIP_EVENTS;

	};

//#ifdef ARDUINO_ARCH_AVR

	class AVRLedStripDriver : public ILedStripDriver {
	public:
		AVRLedStripDriver();
		AVRLedStripDriver(uint8_t driverPin);

		void configDriverPin(uint8_t driverPin);

		inline int setNextLed(ColourRGB *);
		int show();
		int show(ColourRGB *, uint16_t numLed);

		uint16_t getRefreshTime_us(uint16_t numLed);

	private:
		uint8_t driverPin;
		uint8_t driverPinPort;
		uint8_t portBit;

		unsigned long lastUpdateTime_ms;

		void sendByte(uint8_t value);
		inline void sendBit(bool isSet);
	};

//#endif

#ifdef ARDUINO_ARCH_ESP32
#include "driver/rmt.h"


	class Esp32LedStripDriver : public ILedStripDriver
	{
	public:
		Esp32LedStripDriver();
		Esp32LedStripDriver(uint8_t pinNum, uint16_t numLeds);
		~Esp32LedStripDriver();
		int config(uint8_t pinNum, uint16_t numLeds);

		int show(ColourRGB *, uint16_t numLed);

		int setNextLed(ColourRGB *);
		int show();


		uint16_t getRefreshTime_us(uint16_t numLed);

	private:
		rmt_item32_t item_high, item_low, item_show;
		rmt_item32_t * itemArray = NULL;
		uint16_t numItems = 0;
		uint16_t itemIter = 0;
		uint8_t pinNum = 0;
		rmt_channel_t rmtChannelNum = rmt_channel_t::RMT_CHANNEL_MAX;

		unsigned long lastUpdateTime_ms;

		int configRmtItems(uint16_t numLeds);
		int selectRmtChannel();
		int configRmtChannel(rmt_channel_t channelNum, uint8_t pinNum);

		int byte2Items(rmt_item32_t * itemBuf, uint16_t numItems, uint8_t byte);
		int rgb2Items(rmt_item32_t * itemBuf, uint16_t numItems, ColourRGB * rgb);
		int ledString2Items(rmt_item32_t * itemBuf, uint16_t numItems, ColourRGB * rgb, uint16_t numLeds);
	};
#endif
}

#endif