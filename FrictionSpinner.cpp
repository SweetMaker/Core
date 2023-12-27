#include "FrictionSpinner.h"

using namespace SweetMaker;

#ifndef abs
#define abs(val) ((val)>0?(val):(-val))
#endif

FrictionSpinner::FrictionSpinner()
{
	rotation_100000 = 0;
	angularVel_100000 = 0;
	torque_512 = 0;
	limitMax = 16384;
	limitMin = -16384;

	inertia_512 = 2048; // small fly wheel
	staticFriction_512 = 510;
	dynamicFriction_512 = 125;
}

void FrictionSpinner::configLimits(int16_t max, int16_t min)
{

	if (min > max)
		return;

	limitMax = max;
	limitMin = min;

	if (rotation_100000 < min)
		rotation_100000 = min;

	if (rotation_100000 > max)
		rotation_100000 = max;
}

void FrictionSpinner::applyTorque_512(int16_t _torque)
{
	torque_512 = _torque;
}

void SweetMaker::FrictionSpinner::reset()
{
	rotation_100000 = 0;
	angularVel_100000 = 0;
	torque_512 = 0;
}


void FrictionSpinner::update(uint16_t elapsedTime_ms)
{
	int32_t friction_512 = 0;

	if ((angularVel_100000 == 0) && (abs(torque_512) < staticFriction_512))
		return;

	if (angularVel_100000 > 0) {
		friction_512 = (int32_t)-dynamicFriction_512;
	}
	else if (angularVel_100000 < 0) {
    	friction_512 = (int32_t)dynamicFriction_512;
	}

	/*
	* Update angular velocity first
	*/
	int32_t deltaV_100000 = (100*(int32_t)(torque_512 + friction_512) * (int32_t)elapsedTime_ms )/ ((int32_t)inertia_512);
	angularVel_100000 += (int16_t)deltaV_100000;

	/*
	* Update position
	*/
	rotation_100000 += (angularVel_100000 * elapsedTime_ms) / 1000;
}

