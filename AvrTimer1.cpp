/*******************************************************************************
    AvrTimer.cpp: Class for interfacing to Avr328p Timer1

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
   1      23-Feb-2016   Initial release
   2      28-Mar-2019   Added getPeriod_us
*******************************************************************************/

#ifdef ARDUINO_AVR_NANO

#include "AvrTimer1.h"


using namespace SweetMaker;

void AvrTimer1::setCountRegister(uint16_t value)
{
	TCNT1 = value;
}
void AvrTimer1::setOutputCompareRegisterA(uint16_t value)
{
	OCR1A = value;
}
void AvrTimer1::setOutputCompareRegisterB(uint16_t value)
{
	OCR1B = value;
}
void AvrTimer1::setControlRegisterA(uint8_t bitMask)
{
	TCCR1A = bitMask;
}
void AvrTimer1::setControlRegisterB(uint8_t bitMask)
{
	TCCR1B = bitMask;
}
void AvrTimer1::setControlRegisterC(uint8_t bitMask)
{
	TCCR1C = bitMask;
}
void AvrTimer1::setInterruptMaskRegister(uint8_t bitMask)
{
	TIMSK1 = bitMask;
}
void AvrTimer1::setInterruptFlagRegister(uint8_t bitMask)
{
	TIFR1 = bitMask;
}
void AvrTimer1::setInputCaptureRegister(uint16_t value)
{
	ICR1 = value;
}

uint16_t AvrTimer1::getCountRegister()
{
	return TCNT1;
}
uint16_t AvrTimer1::getOutputCompareRegisterA()
{
	return OCR1A;
}
uint16_t AvrTimer1::getOutputCompareRegisterB()
{
	return OCR1B;
}
uint8_t AvrTimer1::getControlRegisterA()
{
	return TCCR1A;
}
uint8_t AvrTimer1::getControlRegisterB()
{
	return TCCR1B;
}
uint8_t AvrTimer1::getControlRegisterC()
{
	return TCCR1C;
}
uint8_t AvrTimer1::getInterruptMaskRegister()
{
	return TIMSK1;
}
uint8_t AvrTimer1::getInterruptFlagRegister()
{
	return TIFR1;
}
uint16_t AvrTimer1::getInputCaptureRegister()
{
	return ICR1;
}

void AvrTimer1::setClockSelectMode(CLOCK_SELECT_MODES csMode)
{
	TCCR1B = (TCCR1B & 0xf8) | csMode;
}

AvrTimer1::CLOCK_SELECT_MODES AvrTimer1::getClockSelectMode()
{
	return (AvrTimer1::CLOCK_SELECT_MODES (TCCR1B & 0x07));
}

void AvrTimer1::setWaveformGenerationMode(AvrTimer1::WAVEFORM_GENERATION_MODE wgm)
{
	TCCR1A = (TCCR1A & 0xf0) | (wgm & 0x3);
	TCCR1B = (TCCR1B & 0xc7) | ((wgm & 0xc) << 1);
}

void AvrTimer1::setWaveformGenerationMode(AvrTimer1::WAVEFORM_GENERATION_MODE wgm, AvrTimer1::OUTPUT_MODE oc1a_mode, AvrTimer1::OUTPUT_MODE oc1b_mode)
{
	uint8_t tccr1a, tccr1b;

	tccr1a = wgm & 0x3;
	switch(oc1a_mode)
	{
		case PIN_DISCONNECTED :
			break;
		case TOGGLE_PIN :
			tccr1a |= 0x10;
			break;
		case CLEAR_PIN :
		case NON_INVERTED_PWM :
			tccr1a |= 0x20;
			break;
		case SET_PIN :
		case INVERTED_PWM :
			tccr1a |= 0x30;
			break;
	}

	switch(oc1b_mode)
	{
		case PIN_DISCONNECTED :
			break;
		case TOGGLE_PIN :
			tccr1a |= 0x40;
			break;
		case CLEAR_PIN :
		case NON_INVERTED_PWM :
			tccr1a |= 0x80;
			break;
		case SET_PIN :
		case INVERTED_PWM :
			tccr1a |= 0xc0;
			break;
	}

	TCCR1A = tccr1a;
	TCCR1B = (TCCR1B & 0xc7) | ((wgm & 0xc) << 1);
}


AvrTimer1::WAVEFORM_GENERATION_MODE getWaveformGenerationMode()
{
	uint8_t wgm = TCCR1A & 0x3;
	wgm |= (TCCR1B & 0x18) >> 1;
	return ((AvrTimer1::WAVEFORM_GENERATION_MODE)wgm);
}



void AvrTimer1::setPeriod_us(WAVEFORM_GENERATION_MODE mode, uint32_t period_us)
{
	AvrTimer1::CLOCK_SELECT_MODES csMode = NO_PRESCALING;
	uint32_t topValue = 0 ;

	switch(mode)
	{
		case PWM_PHASE_AND_FREQUENCY_CORRECT_ICR:
		case PWM_PHASE_AND_FREQUENCY_CORRECT_OCRA:

			// these calculations presume F_CPU == 16000000
			if(period_us <= 8192) //(2 * 65536 * 1000000 / F_CPU)
			{
				csMode = NO_PRESCALING;
				topValue = period_us << 3; // F_CPU / (2 * 1000000);
			}

			else if (period_us <= 65536) // (2 * 8 * 65536 * 1000000 / F_CPU)
			{
				csMode = PRESCALING_8;
				topValue = period_us; // (F_CPU / (2 * 1000000 * 8)
			}
			else if (period_us <= (524288)) // (2 * 64 * 65536 * 1000000 / F_CPU)
			{
				csMode = PRESCALING_64;
				topValue = period_us >> 3; // (F_CPU / (2 * 1000000 * 64)
			}
			else if (period_us <= 2097152) // (2 *256 * 65536 * 1000000 / F_CPU)
			{
				csMode = PRESCALING_256;
				topValue = period_us >> 5; // (F_CPU / (2 * 1000000 * 256)
			}
			else if (period_us <= 8388608) // (2 * 1024 * 65536 * 1000000 / F_CPU)
			{
				csMode = PRESCALING_1024;
				topValue = period_us >> 7; // (F_CPU / (2 * 1000000 * 1024)
			}
			break;

	}
	/*
	 * When in PWM_PHASE_AND_FREQUENCY_CORRECT_ICR mode the ICR pin holds TOP
	 */
	AvrTimer1::setInputCaptureRegister(topValue);
	AvrTimer1::setClockSelectMode(csMode);

	if (AvrTimer1::getCountRegister() > topValue) 
		AvrTimer1::setCountRegister(topValue);

	/*
	 * By default the OC Registers are set to 50% duty by dividing by 2 (right shift 1)
	 */
	AvrTimer1::setOutputCompareRegisterA(topValue >> 1);
}


uint32_t AvrTimer1::getPeriod_us()
{
	CLOCK_SELECT_MODES clockSelectMode = getClockSelectMode();
	uint16_t inputCaptureRegister = getInputCaptureRegister();
	uint32_t period_us;
	switch (clockSelectMode) {
	case PRESCALING_8:
		period_us = inputCaptureRegister;
		break;
	case PRESCALING_64:
		period_us = inputCaptureRegister << 3;
		break;
	case PRESCALING_256:
		period_us = inputCaptureRegister << 5;
		break;
	case PRESCALING_1024:
		period_us = inputCaptureRegister << 7;
		break;
	}
	return(period_us);
}

#endif