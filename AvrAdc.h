/*******************************************************************************
AvrAdc.h API for controlling Avr ADC

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
   1      25-Nov-2018   Initial release
*******************************************************************************/


#ifdef ARDUINO_AVR_NANO

#ifndef __AVR_ADC_H__
#define __AVR_ADC_H__

#include <Arduino.h>

namespace SweetMaker
{

	class AvrAdc
	{
	public:
		typedef enum {
			ADCP_LOW_U8,
			ADCP_MEDIUM_U10,
			ADCP_HIGH_U10
		}ADC_PRECISION;

		static void enableAdc();
		static void disableAdc();
		static void startConversion(uint16_t inputRef);
		static bool isConverting();
		static void setPrecision(ADC_PRECISION);
		static void setInterruptHandler(void *func(void));
		static void enableInterrupt(void);
		static void disableInterrupt(void);
		static void setAutoTrigger(uint16_t trigger);

		static void setRefVoltageSource();

		static uint16_t getResult_u16();
		static uint8_t getResult_u8();

		typedef enum AdcMultiplexerSelectionRegister
		{
			REFERENCE_SELECTION_BIT_1 = _BV(REFS1),
			REFERENCE_SELECTION_BIT_0 = _BV(REFS0),
			ADC_LEFT_ADJUST_RESULT = _BV(ADLAR),
			ANALOG_CHANNEL_SELECTION_BIT_0 = _BV(MUX0),
			ANALOG_CHANNEL_SELECTION_BIT_1 = _BV(MUX1),
			ANALOG_CHANNEL_SELECTION_BIT_2 = _BV(MUX2),
			ANALOG_CHANNEL_SELECTION_BIT_3 = _BV(MUX3),
		} ADC_MULTIPLEXER_SELECTION_REGISTER;

		typedef enum AdcControlAndStatusRegisterA
		{
			ADC_ENABLE = _BV(ADEN),
			ADC_START_CONVERSION = _BV(ADSC),
			ADC_AUTO_TRIGGER_ENABLE = _BV(ADATE),
			ADC_INTERRUPT_FLAG = _BV(ADIF),
			ADC_INTERRUPT_ENABLE = _BV(ADIE),
			ADC_PRESCALER_SELECT_BIT0 = _BV(ADPS0),
			ADC_PRESCALER_SELECT_BIT1 = _BV(ADPS1),
			ADC_PRESCALER_SELECT_BIT2 = _BV(ADPS2),
		} ADC_CONTROL_AND_STATUS_REGISTER_A;

		typedef enum AdcControlAndStatusRegisterB
		{
			ADC_ACME = _BV(ACME),
			ADC_AUTO_TRIGGER_SOURCE_0 = _BV(ADTS0),
			ADC_AUTO_TRIGGER_SOURCE_1 = _BV(ADTS1),
			ADC_AUTO_TRIGGER_SOURCE_2 = _BV(ADTS2),
		} ADC_CONTROL_AND_STATUS_REGISTER_B;

	};


}
#endif

#endif