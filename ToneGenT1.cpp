/*******************************************************************************
    ToneGen.h Implementation of IToneGen using Timer1 and the OC pins

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

#include "Arduino.h"
#include "ToneGenT1.h"


using namespace SweetMaker;
/*
 * This version takes advantage of the 'PWM, phase and frequency corrected' mode
 * which uses ICR1 as the 'TOP' value. Having the frequency controlled by the
 * prescaler and the ICR1 register leaves OCR1A and OCR1B to be used to
 * control the output pins independently.
 *
 * At maximum volume the output pins are toggled at a 50% duty cycle such that one
 * is on and one off at any one time. As the volume is decreased the duty cycle
 * of one is increased and of the other is decreased such that the pins hold the
 * same state for a period of time thus reducing the volume.
 */

ToneGen_T1_OC_Pins::ToneGen_T1_OC_Pins()
{
}

void ToneGen_T1_OC_Pins::setup(void)
{
	pinMode(AvrTimer1::OC1A_PIN, OUTPUT);
	pinMode(AvrTimer1::OC1B_PIN, OUTPUT);
	volume = 0xff;
	AvrTimer1::setWaveformGenerationMode(
	    AvrTimer1::PWM_PHASE_AND_FREQUENCY_CORRECT_ICR,
	    AvrTimer1::INVERTED_PWM,
	    AvrTimer1::NON_INVERTED_PWM
	);

}

/*
 * This configures the timer prescaler and TOP values to achieve the
 * requested frequency. The timer then automatically starts and will
 * toggle OC1A and OC1B
 *
 * The Master clock frequency is F_CPU and expected to be 16M or 8M
 * At 16M the lowest frequecy without prescaling is 16000000 / 0xffff = 244.14
 * With the use of PRESCALING_8 this allows us to reach 30.5Hz which allows
 * support for NOTE_B0 is 31Hz.
 *
 */
void ToneGen_T1_OC_Pins::setFrequency_hz(uint16_t frequency)
{
	uint32_t period_us;
	if(frequency == 0)
	{
		this->stop();
		return;
	}
    period_us = 1000000 / frequency;
    setPeriod_us(period_us);
}

void ToneGen_T1_OC_Pins::setPeriod_us(uint32_t period_us)
{
	AvrTimer1::setPeriod_us(AvrTimer1::PWM_PHASE_AND_FREQUENCY_CORRECT_ICR, period_us);
}

void ToneGen_T1_OC_Pins::writeValue(int32_t period_us)
{
	setPeriod_us(period_us);
}

void ToneGen_T1_OC_Pins::stop(void)
{
   AvrTimer1::setClockSelectMode(AvrTimer1::NO_CLOCK_SOURCE);
}

void ToneGen_T1_OC_Pins::setVolume(uint8_t new_volume)
{
	volume = new_volume;
}

/*
 *************************
 */
 
#endif


