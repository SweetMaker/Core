/*******************************************************************************
    AvrTimer1.h API for controlling Avr Timer 1

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

#ifdef ARDUINO_AVR_NANO

#ifndef __AVR_TIMER_1_H__
#define __AVR_TIMER_1_H__
#include <Arduino.h>

namespace SweetMaker
{

class AvrTimer1
{
public:
	static void setCountRegister(uint16_t value);
	static void setOutputCompareRegisterA(uint16_t value);
	static void setOutputCompareRegisterB(uint16_t value);
	static void setControlRegisterA(uint8_t bitMask);
	static void setControlRegisterB(uint8_t bitMask);
	static void setControlRegisterC(uint8_t bitMask);
	static void setInterruptMaskRegister(uint8_t bitMask);
	static void setInterruptFlagRegister(uint8_t bitMask);
	static void setInputCaptureRegister(uint16_t value);

	static uint16_t getCountRegister();
	static uint16_t getOutputCompareRegisterA();
	static uint16_t getOutputCompareRegisterB();
	static uint8_t getControlRegisterA();
	static uint8_t getControlRegisterB();
	static uint8_t getControlRegisterC();
	static uint8_t getInterruptMaskRegister();
	static uint8_t getInterruptFlagRegister();
	static uint16_t getInputCaptureRegister();

	typedef enum ControlRegisterABitMask
	{
	    WAVEFORM_GENERATION_MODE_BIT0 = _BV(WGM10),
	    WAVEFORM_GENERATION_MODE_BIT1 = _BV(WGM11),
	    COMPARE_OUTPUT_MODE_CHANNEL_B_BIT0 = _BV(COM1B0),
	    COMPARE_OUTPUT_MODE_CHANNEL_B_BIT1 = _BV(COM1B1),
	    COMPARE_OUTPUT_MODE_CHANNEL_A_BIT0 = _BV(COM1A0),
	    COMPARE_OUTPUT_MODE_CHANNEL_A_BIT1 = _BV(COM1A1),
	} CONTROL_REGISTER_A_BIT_MASK;

	typedef enum ControlRegisterBBitMask
	{
	    CLOCK_SELECT_BIT0 = _BV(CS10),
	    CLOCK_SELECT_BIT1 = _BV(CS11),
	    CLOCK_SELECT_BIT2 = _BV(CS12),
	    WAVEFORM_GENERATION_MODE_BIT2 = _BV(WGM12),
	    WAVEFORM_GENERATION_MODE_BIT3 = _BV(WGM13),
	    INPUT_CAPTURE_EDGE_SELECT = _BV(ICES1),
	    INPUT_CAPTURE_NOISE_CANCELER = _BV(ICNC1),
	} CONTROL_REGISTER_B_BIT_MASK;

	typedef enum ControlCBitMask
	{
	    FORCE_OUTPUT_COMPARE_A = _BV(FOC1A),
	    FORCE_OUTPUT_COMPARE_B = _BV(FOC1B)
	} CONTROL_C_BIT_MASK;

	typedef enum InterruptBitMask
	{
	    OVERFLOW_INTERRUPT_ENABLE = _BV(TOIE1),
	    OUTPUT_COMPARE_A_MATCH_INTERRUPT_ENABLE = _BV(OCIE1A),
	    OUTPUT_COMPARE_B_MATCH_INTERRUPT_ENABLE = _BV(OCIE1B),
	    INPUT_CAPTURE_INTERRUPT_ENABLE = _BV(ICIE1)
	} INTERRUPT_BIT_MASK;

	typedef enum InterruptFlagBitMask
	{
	    INPUT_CAPTURE_FLAG = _BV(ICF1),
	    OUTPUT_COMPARE_B_MATCH_FLAG = _BV(OCF1B),
	    OUTPUT_COMPARE_A_MATCH_FLAG = _BV(OCF1A),
	    OVERFLOW_FLAG = _BV(TOV1)
	} INTERRUPT_FLAG_BIT_MASK;

	typedef enum ClockSelectModes
	{
	    NO_CLOCK_SOURCE = 0,
	    NO_PRESCALING = 1,
	    PRESCALING_8 = 2,
	    PRESCALING_64 = 3,
	    PRESCALING_256 = 4,
	    PRESCALING_1024 = 5,
	    EXTERNAL_SOURCE_FALLING_EDGE = 6,
	    EXTERNAL_SOURCE_RISING_EDGE = 7
	} CLOCK_SELECT_MODES;

	typedef enum OutputMode
	{
	    PIN_DISCONNECTED,  // does not set OC1A or OC1B pins
	    TOGGLE_PIN,         // toggles OC1A or OC1B pin
	    CLEAR_PIN,          // clears OC1A or OC1B pin
	    SET_PIN,            // sets OC1A or OC1B pin
	    NON_INVERTED_PWM,   // sets OC1A or OC1B pins
	    INVERTED_PWM        // sets OC1A or OC1B pins
	} OUTPUT_MODE;

	typedef enum WaveformGenerationMode
	{
	    NORMAL = 0,
	    FAST_PWM_8_BIT = 5,
	    FAST_PWM_9_BIT = 6,
	    FAST_PWM_10_BIT = 7,
	    FAST_PWM_ICR = 14,
	    FAST_PWM_OCRA = 15,
	    PWM_PHASE_CORRECT_8_BIT = 1,
	    PWM_PHASE_CORRECT_9_BIT = 2,
	    PWM_PHASE_CORRECT_10_BIT = 3,
	    PWM_PHASE_CORRECT_ICR = 10,
	    PWM_PHASE_CORRECT_OCRA = 11,
	    PWM_PHASE_AND_FREQUENCY_CORRECT_ICR = 8,
	    PWM_PHASE_AND_FREQUENCY_CORRECT_OCRA = 9,
	    CLEAR_TIMER_ON_COMPARE_OCRA = 4,
	    CLEAR_TIMER_ON_COMPARE_ICR = 12
	} WAVEFORM_GENERATION_MODE;

	static void setWaveformGenerationMode(WAVEFORM_GENERATION_MODE wgm);
	static void setWaveformGenerationMode(WAVEFORM_GENERATION_MODE mode,
	                                      OUTPUT_MODE ocA,
	                                      OUTPUT_MODE ocB
	                                     );

	static void setPeriod_us(WAVEFORM_GENERATION_MODE mode, uint32_t period_us);
	static uint32_t getPeriod_us();

	static void setClockSelectMode(CLOCK_SELECT_MODES csMode);
	static CLOCK_SELECT_MODES getClockSelectMode();

	static const uint8_t OC1A_PIN = 9;
	static const uint8_t OC1B_PIN = 10;

};


}
#endif

#endif