/*******************************************************************************
AvrAdc.cpp: Class for interfacing to Avr328p Adc control unit

Copyright(C) 2018 Howard James May

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
1      30-Nov-2018   Initial release
*******************************************************************************/

#ifdef ARDUINO_AVR_NANO

#include "AvrAdc.h"


using namespace SweetMaker;

/*
* To enable ADC disable ADC Power save mode and set ADC Enable bit
*                         This also sets the ADC to default values
*/
void AvrAdc::enableAdc()
{
	PRR &= ~_BV(PRADC);
	ADCSRA |= ADC_ENABLE;
	setPrecision(ADCP_MEDIUM_U10);
}

/*
 * disableAdc - stops all activity and powers the unit down. 
 */
void AvrAdc::disableAdc()
{
	PRR |= _BV(PRADC);
	ADCSRA &= ~ADC_ENABLE;
}

/*
* startConversion - This sets the multiplexer to select the correct input and then
*                   starts the conversion. 
*/
void AvrAdc::startConversion(uint16_t inputRef)
{
	uint8_t mask = (ANALOG_CHANNEL_SELECTION_BIT_0 |
		ANALOG_CHANNEL_SELECTION_BIT_1 |
		ANALOG_CHANNEL_SELECTION_BIT_2 |
		ANALOG_CHANNEL_SELECTION_BIT_3
		);
	ADMUX &= ~mask;
	ADMUX |= (uint8_t)inputRef & mask;
	ADMUX |= ADC_START_CONVERSION;
}

bool AvrAdc::isConverting()
{
	if (ADMUX & ADC_START_CONVERSION)
		return (true);
	return false;
}

void AvrAdc::setPrecision(ADC_PRECISION p)
{
	ADCSRA &= ~(ADC_PRESCALER_SELECT_BIT0 |
		ADC_PRESCALER_SELECT_BIT1 |
		ADC_PRESCALER_SELECT_BIT2);

	switch (p)
	{
	case ADCP_LOW_U8:
		ADCSRA |= 3;
		ADMUX |= ADC_LEFT_ADJUST_RESULT;
		break;

	default:
	case ADCP_MEDIUM_U10:
		ADMUX &= ~ADC_LEFT_ADJUST_RESULT;
		ADCSRA |= 3;
		break;

	case ADCP_HIGH_U10:
		ADMUX &= ~ADC_LEFT_ADJUST_RESULT;
		ADCSRA |= 7;
		break;
	}
}

void AvrAdc::setInterruptHandler(void * func(void))
{ 
//	attachInterrupt(ADC_vect, func);
}

void AvrAdc::enableInterrupt(void)
{
	ADCSRA |= ADC_INTERRUPT_ENABLE;
}

void AvrAdc::disableInterrupt(void)
{
	ADCSRA &= ~ADC_INTERRUPT_ENABLE;
}

void AvrAdc::setAutoTrigger(uint16_t trigger)
{
	//todo
}

/*
 * setRefVoltageSource - if an external reference is applied then this must be set to 
 *                       AREF (0) as is the case with Arduino
 */
void AvrAdc::setRefVoltageSource()
{
	ADMUX &= ~REFERENCE_SELECTION_BIT_1;
	ADMUX &= ~REFERENCE_SELECTION_BIT_0;
}


uint16_t AvrAdc::getResult_u16()
{
	uint16_t result;
	result = ADCL;
	result += ADCH << 8;
	if (ADMUX & ADC_LEFT_ADJUST_RESULT)
		result = result >> 6;

	return (result);
}

uint8_t AvrAdc::getResult_u8()
{
	if (ADMUX & ADC_LEFT_ADJUST_RESULT)
		return ADCH;

	uint8_t result = ADCL >> 2;
	result += ADCH << 6;
	return (result);
}

#endif