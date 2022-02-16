
#define __STDC_LIMIT_MACROS
#include "Stepper.h"

using namespace SweetMaker;

/*
 * This implementation works by dividing the input by a precomputed value the result of which
 * gives the current step unadjusted for hysterises. Hysterisis is only applied when moving up
 * a step and so if the current step is found to be lower then we can immediately update the 
 * current step. If the new step is found to be more than one higher again we can immediately
 * update. If it is one higher then we subtract the hysterisis amount from the input signal and
 * recalculate in order to determine if we have moved to the next step with a sufficient margin.
 */

#ifndef UINT16_MAX
#define UINT16_MAX (0xffffL)
#endif

Stepper::Stepper()
{
	currentStep = 0;
	Cm = 0;
	hysteresis = 0;
	options = 0;
}


Stepper::Stepper(int16_t min, int16_t max, uint16_t num_steps)
{
	Stepper();
	configSteps(min, max, num_steps);
}


void Stepper::configSteps(int16_t min, int16_t max, uint16_t num_steps)
{
	int32_t _Cm;
	this->rangeMin = min;
	rangeSize = max - min;
	numSteps = num_steps;
	this->hysteresis = (rangeSize / num_steps) >> 3;
	if (this->hysteresis == 0)
		this->hysteresis = 1;

	_Cm = UINT16_MAX * (int32_t)num_steps;
	_Cm /= (int32_t)rangeSize;
	Cm = (int16_t)_Cm;
}


void Stepper::configOptions(OPTIONS _options)
{
	options = _options;
}


bool Stepper::update(int16_t reading)
{
	int16_t rawStep = calculateStep(reading);
	int16_t deltaStep = rawStep - currentStep;

	if (!(options & UNLIMITED_STEPS))
	{
		if (rawStep < 0)
			rawStep = 0;
		else if (rawStep >= numSteps)
			rawStep = numSteps - 1;
	}

	if (rawStep == currentStep + 1) {
		rawStep = calculateStep(reading - hysteresis);
	}
	if (rawStep != currentStep)
	{
		currentStep = rawStep;
		return (true);
	}
	return (false);
}


void Stepper::writeValue(int32_t input)
{
	update(input);
}


int16_t SweetMaker::Stepper::calculateStep(int16_t input)
{
	int32_t delta = input - rangeMin;
	delta *= (int32_t)Cm;
	delta /= UINT16_MAX;
	return ((uint16_t)delta);
}



