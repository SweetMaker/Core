#include "Arduino.h"
#include "Drv8833Driver.h"

#include "Platform.h"

SweetMaker::Drv8833Driver::Drv8833Driver()
{
}

void SweetMaker::Drv8833Driver::configMotorA(uint8_t pin1, uint8_t pin2)
{
	channelA_pin1 = pin1;
	channelA_pin2 = pin2;
	pinMode(pin1, OUTPUT);
	pinMode(pin2, OUTPUT);
}

void SweetMaker::Drv8833Driver::configMotorB(uint8_t pin1, uint8_t pin2)
{
	channelB_pin1 = pin1;
	channelB_pin2 = pin2;
	pinMode(pin1, OUTPUT);
	pinMode(pin2, OUTPUT);
}

void SweetMaker::Drv8833Driver::configEnablePin(uint8_t pin)
{
	enablePin = pin;
	pinMode(pin, OUTPUT);
}

void SweetMaker::Drv8833Driver::configFaultPin(uint8_t pin)
{
	faultPin = pin;
	pinMode(pin, INPUT_PULLUP);
}

void SweetMaker::Drv8833Driver::setSpeedA_16384(int16_t speed)
{
	uint16_t absSpeed = (uint16_t)abs(speed);
	absSpeed = absSpeed >> 6;
	absSpeed = 255 - absSpeed;

	Serial.print("SpeedA: ");
	Serial.println(absSpeed);

	if (speed >= 0) {
		digitalWrite(channelA_pin1, HIGH);
		analogWrite(channelA_pin2, absSpeed);
	}
	else {
		analogWrite(channelA_pin1, absSpeed);
		digitalWrite(channelA_pin2, HIGH);
	}
}

void SweetMaker::Drv8833Driver::setSpeedB_16384(int16_t speed)
{
	uint16_t absSpeed = (uint16_t)abs(speed);
	absSpeed = absSpeed >> 6;
	absSpeed = 255 - absSpeed;

	Serial.print("SpeedB: ");
	Serial.println(absSpeed);

	if (speed >= 0) {
		digitalWrite(channelB_pin1, HIGH);
		analogWrite(channelB_pin2, absSpeed);
	}
	else {
		analogWrite(channelB_pin1, absSpeed);
		digitalWrite(channelB_pin2, HIGH);
	}
}

void SweetMaker::Drv8833Driver::setEnable(boolean enable)
{
	if (enable)
		digitalWrite(enablePin, HIGH);
	else
		digitalWrite(enablePin, LOW);
}

bool SweetMaker::Drv8833Driver::getFault()
{
	if(digitalRead(faultPin) == 0)
    	return (true);
	return (false);
}
