/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Blink
*/

#include "Arduino.h"
#include "SwitchDriver.h"
#include "SigGen.h"
#include "LedDriver.h"
#include "PiezoDriver.h"
#include "TunePlayer.h"
#include "TimerMngt.h"
#include "EepromUtility.h"
#include "Colour.h"
#include "EventMngr.h"
#include "Updater.h"
#include "PerfMon.h"
#include "LedStripDriver.h"


using namespace SweetMaker;


void myEventHandler(uint16_t eventId, uint8_t src, uint16_t eventInfo);
void handleSerialInput();

#define LED_PIN 8

static const uint8_t num_lights = 5;
ColourRGB ledStrip[num_lights];
ILedStripDriver* ledStripDriver;
#define LED_STRIP_PIN 4


void setup()
{
	/* Start Serial at a speed (Baud rate) of 112500 Bytes per second */
	Serial.begin(112500);
	Serial.println("esp32-c3-test says Hi");
	pinMode(LED_PIN, OUTPUT);

	EventMngr::getMngr()->configCallBack(myEventHandler);
	TimerTickMngt::getTimerMngt()->update(0);

	pinMode(LED_STRIP_PIN, OUTPUT);
	digitalWrite(LED_STRIP_PIN, LOW);

	ledStripDriver = new Esp32LedStripDriver(LED_STRIP_PIN, 5);

	ledStrip[0] = 0x00BFFF;
	ledStrip[1] = 0xff1493;
	ledStrip[2] = 0x7cfc00;
	ledStrip[3] = 0xffd700;
	ledStrip[4] = 0xf0f8ff;

}

void loop()
{
	static unsigned long lastUpdateTime_ms;

	// Check for any input on the Serial Port (only relevant when connected to computer)
	handleSerialInput();

	unsigned long thisTime_ms = millis();
	unsigned long elapsedTime_ms = thisTime_ms - lastUpdateTime_ms;

	PerfMon::getPerfMon()->intervalStop();
	PerfMon::getPerfMon()->intervalStart();

	AutoUpdateMngr::getUpdater()->update(elapsedTime_ms);

	lastUpdateTime_ms = thisTime_ms;

}

static const SigGen::SAMPLE alarmSignal[] PROGMEM = { 0, 1000 };
SigGen alarmGen(alarmSignal, sizeof(alarmSignal)>>1, 10000, 0);

void myEventHandler(uint16_t eventId, uint8_t eventRef, uint16_t eventInfo)
{
	switch (eventId)
	{
	case SwitchDriver::SWITCH_TURNED_ON: // The button has been pressed
	{
	}
	break;

	case SigGen::SIG_GEN_FINISHED: // A Signal Generator has finished 
	{
		Serial.println("SIG_GEN_FINISHED");
	}
	break;

	case TimerTickMngt::TIMER_EXPIRED: // A timer has expired - eventInfo from timerId
		Serial.println("TimerTickMngt::TIMER_EXPIRED");
		break;

	case TimerTickMngt::TIMER_TICK_S: // Generated every second
		break;

	case TimerTickMngt::TIMER_TICK_100MS: // Generated ten times a second
		if (alarmGen.isRunning()) {
			Serial.println(alarmGen.readValue());
		}
		break;


	case SwitchDriver::SWITCH_TURNED_OFF: // The switch has been released
	case SwitchDriver::SWITCH_HELD_ON:   // The button has been held down for a second
	case SwitchDriver::SWITCH_STILL_HELD_ON: // Held down for another second - eventInfo counts number of seconds
	case SigGen::SIG_GEN_STARTED: // A Signal Generator has been started
	case SigGen::SIG_GEN_STOPPED: // A Signal Generator has been stopped
	case TimerTickMngt::TIMER_TICK_UPDATE: // Generated every time fizzyMint is updated - could be every 500us (micro seconds) e.g. less than a millisecond
	case TimerTickMngt::TIMER_TICK_10S: // Generated once every ten seconds
	case TimerTickMngt::TIMER_FREQ_GEN: // Generated a certain number of times a seconds
	case TunePlayer::TUNE_NEXT_NOTE: // Tune Player started next note
	case TunePlayer::TUNE_STOPPED: // The Tune has been stopped before it has ended
	case TunePlayer::TUNE_ENDED: // The Tune has ended
		break;
	}
}


/* This supports various management functions as shown below */
void handleSerialInput() {
	if (Serial.available()) {
		char c = Serial.read();
		Serial.println(c);

		switch (c) {
		case 'a': {
			Serial.println("Flash led once");
			digitalWrite(LED_PIN, HIGH);  // turn the LED on (HIGH is the voltage level)
			delay(1000);                      // wait for a second
			digitalWrite(LED_PIN, LOW);   // turn the LED off by making the voltage LOW
		}
		break;

		case 'l': {
			Serial.println("Show Led Strip");
			cli();
			int retVal = ledStripDriver->show(ledStrip, 5);
			sei();
			if ((retVal != 0) && (retVal != -1))
				Serial.println(retVal);
		}
		break;


		case 't': {
			Serial.println("Start timer");
			static Timer tim;
			tim.startTimer(1000, 12);
		}
		break;

		case 's': {
			Serial.println("Start sigGen");
			alarmGen.start(1);
		}
		break;
		}
	}
}
