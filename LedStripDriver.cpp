/*******************************************************************************
LedStripDriver.cpp - Implements control protocol for WS2812B LED Strips

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
   1      3-May-2019    Initial release
*******************************************************************************/

#include <Arduino.h>
#include "LedStripDriver.h"
#include "EventMngr.h"

/*
 * Timing constants for WS2812B
 */
	 
#define WS2812B_CLOCK_PERIOD_NS (208)
#define WS2812B_T1H_NS (WS2812B_CLOCK_PERIOD_NS * 3 + 1)
#define WS2812B_T1L_NS (WS2812B_CLOCK_PERIOD_NS * 2 + 1)
#define WS2812B_T0H_NS (WS2812B_CLOCK_PERIOD_NS  + 1)
#define WS2812B_T0L_NS (WS2812B_CLOCK_PERIOD_NS * 3 + 1)
#define WS2812B_RESET_NS (6000)


using namespace SweetMaker;

#ifdef ARDUINO_ARCH_AVR

const static uint8_t avrPort = PORTB;
const static uint8_t portPinNum = 3;

#define CYCLE_TIME_NS ( 1000000000L / F_CPU )
#define NS_2_CYCLES(t_ns) ( (t_ns) / CYCLE_TIME_NS )

/* 
 * AVRLedStripDriver
 */
AVRLedStripDriver::AVRLedStripDriver() {
}

/*
 * LedStripDriver - configure a new instance with the pin number to use
 */
AVRLedStripDriver::AVRLedStripDriver(uint8_t _driverPin) {
	configDriverPin(_driverPin);
}

void AVRLedStripDriver::configDriverPin(uint8_t _driverPin)
{
	driverPin = _driverPin;
	pinMode(_driverPin, OUTPUT);

	/*
	* The pin must be mapped to internal port and register
	*/
	switch (_driverPin)
	{
	case 4:
		driverPinPort = PORTB;
		portBit = 4;
		break;
	case 11:
		driverPinPort = PORTB;
		portBit = 3;
		break;

	default:
		break;
	}
}

/*
 * sendBit - this is a time critical function with precision needed when the signal tranistions
 *           from high to low. A '1' bit is high for T1H and low for T1L ns. A '0' bit is high for
 *           T0H and low for T0L ns. Time between bits is not critical but must be less than the frame
 *           latch time (RES) which tells the Led Strip we have finished sending data and it should
 *           display the output.
 */
inline void AVRLedStripDriver::sendBit(bool isSet) {

	if (isSet) {
		/* 
		 * Output a '1' (T1H then T1L)
		 */
		asm volatile (
			"sbi %[port], %[bit] \n\t"	// Set output high 
			".rept %[onCycles] \n\t"    // Delay T1H by repeated 'no operation' instructions
			"nop \n\t"                  // ..
			".endr \n\t"                // ..
			"cbi %[port], %[bit] \n\t"  // Set output low
			".rept %[offCycles] \n\t"   // Delay T1L by repeated 'no operation' instructions
			"nop \n\t"                  // ..
			".endr \n\t"                // .. Done
			::
			[port]		"I" (_SFR_IO_ADDR(PORTB)),  // this finds the memory address for the port given the portNum
			[bit]		"I" (3),                      // 
			[onCycles]	"I" (NS_2_CYCLES(WS2812B_T1H_NS) - 2),		// we can remove a couple of clock cycles on account of overhead in the function
			[offCycles] 	"I" (NS_2_CYCLES(WS2812B_T1L_NS) - 2)		// again we can remove a couple of clock cycles on account of overhead
			);
	}
	else {
		/*
		* Output a '0' (T0H then T0L)
		*/
		asm volatile (
			"sbi %[port], %[bit] \n\t"	// Set the output high
			".rept %[onCycles] \n\t"    // Delay T0H by repeating 'no operation' instructions
			"nop \n\t"                  // ..
			".endr \n\t"                // ..
			"cbi %[port], %[bit] \n\t"  // Set output low
			".rept %[offCycles] \n\t"   // Delay T0L by repeating 'no operation' instructions
			"nop \n\t"                  // ..
			".endr \n\t"                // .. Done
			::
			[port]		"I" (_SFR_IO_ADDR(PORTB)),
			[bit]		"I" (3),
			[onCycles]	"I" (NS_2_CYCLES(WS2812B_T0H_NS) - 2),
			[offCycles]	"I" (NS_2_CYCLES(WS2812B_T0L_NS) - 2)
			);
	}
}

/*
 * sendByte - Ws8102B requires bytes sent most significant bit first
 */
inline void AVRLedStripDriver::sendByte(uint8_t byte) {

	for (uint8_t bit_mask = 0x80; bit_mask != 0; bit_mask = bit_mask >> 1) {
		if (byte & bit_mask)
			sendBit(1);                
		else	                 		
			sendBit(0);
	}
}

/* 
 * setNextLed - Takes a ColourRGB and outputs the individual RGB values to the strip.
 *              This can be called successively to output values without using a long
 *              array of ColourRGB. The delay between calls Must be less than the strip
 *              reset time of 5us. 
 */
inline int AVRLedStripDriver::setNextLed(ColourRGB * rgb) {

	sendByte(rgb->green);          // Neopixel wants colors in green then red then blue order
	sendByte(rgb->red);
	sendByte(rgb->blue);
	return(0);
}

/*
 * show - Presuming the driver pin has been left low (always is by these functions) 
 *        then we only have to wait RES ns
 */
int AVRLedStripDriver::show()
{
	_delay_us(WS2812B_RESET_NS / 1000UL);
	return(0);
}

/*
* show - Output the values from the passed in array of numLed values then show
*      - This places a limit on the refresh rate at 80Hz
*/
int AVRLedStripDriver::show(ColourRGB *rgb, uint16_t numLed)
{
	unsigned long currentTime_ms = millis();
	unsigned long elapsedTime_ms = currentTime_ms - lastUpdateTime_ms;
	if (elapsedTime_ms < 25)
		return (0);
	lastUpdateTime_ms = currentTime_ms;

	for (; numLed > 0;  numLed--)
		setNextLed(rgb++);

	return (show());
}

uint16_t AVRLedStripDriver::getRefreshTime_us(uint16_t numLed)
{
	return (40 * numLed + 10);
}

#endif

#ifdef ARDUINO_ARCH_ESP32

#define F_APB_CLOCK_HZ		(80000000L)
#define APB_CLOCK_PERIOD_NS ( 1000000000L / F_APB_CLOCK_HZ )
#define RMT_DIVIDER			(2)
#define RMT_CLOCK_PERIOD_NS (APB_CLOCK_PERIOD_NS * RMT_DIVIDER)


static uint8_t activeChannelBitMap = 0;

Esp32LedStripDriver::Esp32LedStripDriver()
{

}

SweetMaker::Esp32LedStripDriver::Esp32LedStripDriver(uint8_t pinNum, uint16_t numLeds)
{
	config(pinNum, numLeds);
}

Esp32LedStripDriver::~Esp32LedStripDriver()
{
	activeChannelBitMap &= ~(uint8_t)rmtChannelNum;
}

int Esp32LedStripDriver::config(uint8_t _pinNum, uint16_t numLeds)
{
	pinNum = _pinNum;

	if (selectRmtChannel())
		return (-1);

	configRmtItems(numLeds);
	configRmtChannel(rmtChannelNum, _pinNum);
	return(0);
}

int Esp32LedStripDriver::configRmtItems(uint16_t numLeds)
{

#define BITS_IN_BYTE (8)
#define BYTES_IN_LED (3)

	this->numItems = (numLeds * BITS_IN_BYTE * BYTES_IN_LED) + 1;

	itemArray = (rmt_item32_t*)malloc(sizeof(rmt_item32_t) * numItems);
	if (itemArray == NULL)
		return (-1);

	item_high.level0 = 1;
	item_high.duration0 = (WS2812B_T1H_NS / RMT_CLOCK_PERIOD_NS) + 1;
	item_high.level1 = 0;
	item_high.duration1 = (WS2812B_T1L_NS / RMT_CLOCK_PERIOD_NS) + 1;

	item_low.level0 = 1;
	item_low.duration0 = (WS2812B_T0H_NS / RMT_CLOCK_PERIOD_NS) + 1;
	item_low.level1 = 0;
	item_low.duration1 = (WS2812B_T0L_NS / RMT_CLOCK_PERIOD_NS) + 1;

	item_show.level0 = 0;
	item_show.level1 = 0;
	item_show.duration0 = 0;
	item_show.duration1 = (WS2812B_RESET_NS / RMT_CLOCK_PERIOD_NS);

	return(0);
}

int Esp32LedStripDriver::selectRmtChannel()
{
	uint8_t bitMask = 0x01;
	for (uint8_t channelNum = 0; channelNum < 8; channelNum++)
	{
		if (activeChannelBitMap & bitMask) {
			bitMask = bitMask << 1;
			continue;
		}
		this->rmtChannelNum = (rmt_channel_t)channelNum;
		activeChannelBitMap |= bitMask;
		return (0);
	}

	return (-1);
}


int Esp32LedStripDriver::configRmtChannel(rmt_channel_t channelNum, uint8_t pinNum)
{
	esp_err_t ret_val;
	rmt_config_t rmtConfig;
	memset(&rmtConfig, 0, sizeof(rmt_config_t));

	rmtConfig.channel = channelNum;
	rmtConfig.gpio_num = (gpio_num_t)27;
	rmtConfig.mem_block_num = 1;
	rmtConfig.rmt_mode = RMT_MODE_TX;
	rmtConfig.clk_div = RMT_DIVIDER;
	rmtConfig.tx_config.carrier_en = false;
	rmtConfig.tx_config.loop_en = false;
	rmtConfig.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
	rmtConfig.tx_config.idle_output_en = true;

	ret_val = rmt_config(&rmtConfig);
	if (ret_val != ESP_OK)
	{
		EventMngr::getMngr()->handleEvent(ILedStripDriver::LSE_CONFIG_ERR, pinNum, 0);
		return(-1);
	}

	ret_val = rmt_driver_install(this->rmtChannelNum, 0, 0);
	if (ret_val != ESP_OK)
	{
		EventMngr::getMngr()->handleEvent(ILedStripDriver::LSE_CONFIG_ERR, pinNum, 0);
		return(-1);
	}
	return(0);
}


int Esp32LedStripDriver::byte2Items(rmt_item32_t * itemBuf, uint16_t numItems, uint8_t byte)
{
	if ((numItems < 8) || (itemBuf == NULL))
		return (-1);

	for (uint8_t bit_mask = 0x80; bit_mask != 0; bit_mask = bit_mask >> 1) {
		if (byte & bit_mask)
			*itemBuf++ = item_high;
		else
			*itemBuf++ = item_low;
	}
	return(0);
}


int Esp32LedStripDriver::rgb2Items(rmt_item32_t * itemBuf, uint16_t numItems, ColourRGB * rgb)
{
	if ((numItems < 8 * 3) || (itemBuf == NULL))
		return (-1);

	byte2Items(itemBuf, numItems, rgb->green);
	itemBuf += 8;
	numItems -= 8;

	byte2Items(itemBuf, numItems, rgb->red);
	itemBuf += 8;
	numItems -= 8;

	byte2Items(itemBuf, numItems, rgb->blue);
	return(0);
}



int Esp32LedStripDriver::ledString2Items(rmt_item32_t * itemBuf, uint16_t numItems, ColourRGB * rgbString, uint16_t numLeds)
{
	uint16_t requiredItems = 8 * 3 * numLeds;
	if ((numItems < requiredItems) || (itemBuf == NULL))
		return(-1);

	for (int i = 0; i < numLeds; i++)
	{
		rgb2Items(itemBuf, numItems, rgbString++);
		itemBuf += 24;
		numItems -= 24;
	}
	return(requiredItems);
}

int Esp32LedStripDriver::show(ColourRGB * ledString, uint16_t numLeds)
{
	uint16_t usedItems;
	esp_err_t ret_val;

	unsigned long thisTime_ms = millis();
	uint16_t elapsedTime_ms = thisTime_ms - lastUpdateTime_ms;
	if (elapsedTime_ms < 10)
		return(-1);

	ret_val = rmt_wait_tx_done(rmtChannelNum, 0);
	if (ret_val == ESP_ERR_TIMEOUT)
		return(-2);

	lastUpdateTime_ms = thisTime_ms;

	usedItems = ledString2Items(itemArray, numItems, ledString, numLeds);
	if (usedItems >= numItems)
		return(-3);

	itemArray[usedItems++] = item_show;

	ret_val = rmt_write_items(rmtChannelNum, itemArray, usedItems, false);
	if (ret_val != 0)
	{
		EventMngr::getMngr()->handleEvent(ILedStripDriver::LSE_SHOW_ERR, pinNum, 0);
		return(-4);
	}

	return(0);
}

int Esp32LedStripDriver::setNextLed(ColourRGB *rgb)
{
	uint16_t availableItems = numItems - itemIter;
	return (rgb2Items(itemArray + itemIter++, availableItems, rgb));
}

int Esp32LedStripDriver::show()
{
	int ret_val;
	if (itemIter >= numItems)
		return(-1);

	itemArray[itemIter++] = item_show;

	ret_val = rmt_write_items(rmtChannelNum, itemArray, itemIter, true);
	if (ret_val != 0)
	{
		EventMngr::getMngr()->handleEvent(ILedStripDriver::LSE_SHOW_ERR, pinNum, 0);
		return(-1);
	}
	itemIter = 0;
	return (0);
}

uint16_t SweetMaker::Esp32LedStripDriver::getRefreshTime_us(uint16_t numLed)
{
	return (0);
}

#endif