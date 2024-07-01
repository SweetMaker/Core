#include <stdio.h>
#include <Arduino.h>
#include "ToDiscrete.h"
#include "EventMngr.h"

using namespace SweetMaker;

ToDiscrete::ToDiscrete()
{
	this->started = false;
	this->current_continuous_value = 0;
	this->current_discrete_value = 0;
	this->output = NULL;
	this->hysteresis_size = 0;
	this->instance_id = 0;
	this->in_step_value = 0;
	this->next_down_threshold = 0;
	this->next_up_threshold = 0;
	this->step_size = 0;
	this->distance_to_mid_255 = 0;
	this->distance_to_mid_scaler = 0;
}

ToDiscrete::ToDiscrete(uint16_t _step_size, uint16_t _hysteresis_size)
{
	ToDiscrete();
	this->step_size = _step_size;
	this->hysteresis_size = _hysteresis_size;
	this->distance_to_mid_scaler = ((uint32_t)0x10000 / _step_size);
}

ToDiscrete::ToDiscrete(uint16_t _step_size, uint16_t _hysteresis_size, uint8_t id)
{
	ToDiscrete();
	this->step_size = _step_size;
	this->hysteresis_size = _hysteresis_size;
	this->instance_id = id;
	this->distance_to_mid_scaler = ((uint32_t)0x10000 / _step_size);
}


void ToDiscrete::writeValue(int32_t value)
{
	if (!this->started) {
		this->start(value);
		return;
	}

	this->current_continuous_value = value;
	this->distance_to_mid_255 = calculateDistanceToMid();

	while (value > this->next_up_threshold) {
		this->next_up_threshold += this->step_size;
		this->next_down_threshold += this->step_size;
		this->current_discrete_value++;
		EventMngr::getMngr()->handleEvent(TO_DISCRETE_EVENT::NEW_VALUE, this->instance_id, this->current_discrete_value);
	}

	while (value < this->next_down_threshold) {
		this->next_up_threshold -= this->step_size;
		this->next_down_threshold -= this->step_size;
		this->current_discrete_value--;
		EventMngr::getMngr()->handleEvent(TO_DISCRETE_EVENT::NEW_VALUE, this->instance_id, this->current_discrete_value);
	}

	this->in_step_value = this->current_continuous_value - this->current_discrete_value * this->step_size;
}

int32_t ToDiscrete::readValue()
{
	return this->in_step_value;
}

void ToDiscrete::start(int32_t current_value)
{
	this->started = true;
	current_continuous_value = current_value;
	current_discrete_value = current_value / step_size;
	next_up_threshold = (current_discrete_value + 1) * step_size + hysteresis_size;
	next_down_threshold = current_discrete_value * step_size - hysteresis_size;
}

void ToDiscrete::stop() {
	this->started = false;
}

/*
 * calculateDistanceToMid - returns 0 when at the mid, 255 when furthest from mid
 */
uint16_t ToDiscrete::calculateDistanceToMid() {
	int16_t mid_val = this->step_size >> 1;
	uint32_t delta = abs(this->in_step_value - mid_val);
	delta = (this->distance_to_mid_scaler * delta);
	delta = delta >> 7;
	delta = delta > 255 ? 255 : delta;
	return delta;
}


void ToDiscrete::print() {
	Serial.print(this->current_discrete_value); Serial.print("\t");
	Serial.print(this->next_down_threshold); Serial.print("\t");
	Serial.print(this->next_up_threshold); Serial.print("\t");
	Serial.print(this->distance_to_mid_255); Serial.print("\t");
}
