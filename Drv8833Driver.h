
#ifndef __DRV8833_DRIVER__
#define __DRV8833_DRIVER__

#include "stdint.h"

namespace SweetMaker
{
	class Drv8833Driver
	{
	public:
		Drv8833Driver();
		void configMotorA(uint8_t pin1, uint8_t pin2);
		void configMotorB(uint8_t pin1, uint8_t pin2);
		void configEnablePin(uint8_t pin);
		void configFaultPin(uint8_t pin);
		void setSpeedA_16384(int16_t speed);
		void setSpeedB_16384(int16_t speed);
		void setEnable(boolean enable);
		bool getFault();

		const static uint8_t NULL_PIN = 0xff;

	private:
		uint8_t channelA_pin1 = NULL_PIN;
		uint8_t channelA_pin2 = NULL_PIN;
		uint8_t channelB_pin1 = NULL_PIN;
		uint8_t channelB_pin2 = NULL_PIN;
		uint8_t enablePin = NULL_PIN;
		uint8_t faultPin = NULL_PIN;
	};


}


#endif