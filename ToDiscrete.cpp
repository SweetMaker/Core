#include <stdio.h>
#include <Arduino.h>
#include "ToDiscrete.h"
#include "EventMngr.h"

using namespace SweetMaker;

ToDiscrete::ToDiscrete()
{
	this->current_continuous_value = 0;
	this->current_discrete_value = 0;
	this->output = NULL;
	this->hysteresis_size = 0;
	this->instance_id = 0;
	this->in_step_value = 0;
	this->next_down_threshold = 0;
	this->next_up_threshold = 0;
	this->step_size = 0;
}

ToDiscrete::ToDiscrete(int32_t step_size, int32_t hysteresis_size)
{
	ToDiscrete();
	this->step_size = step_size;
	this->hysteresis_size = hysteresis_size;
}


void ToDiscrete::writeValue(int32_t value)
{
	this->current_continuous_value = value;

	while (value > this->next_up_threshold) {
		this->next_up_threshold += this->step_size;
		this->next_down_threshold += this->step_size;
		this->current_discrete_value++;
		Serial.println(value);
		EventMngr::getMngr()->handleEvent(TO_DISCRETE_EVENT::NEW_VALUE, this->instance_id, this->current_discrete_value);
	}

	while (value < this->next_down_threshold) {
		this->next_up_threshold -= this->step_size;
		this->next_down_threshold -= this->step_size;
		this->current_discrete_value--;
		Serial.println(value);
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
	Serial.println("ToDiscrete::start");
	current_continuous_value = current_value;
	current_discrete_value = current_value / step_size;
	next_up_threshold = (current_discrete_value + 1) * step_size + hysteresis_size;
	next_down_threshold = current_discrete_value * step_size - hysteresis_size;
}

void ToDiscrete::stop()
{
}
