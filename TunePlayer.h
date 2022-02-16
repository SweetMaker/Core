/*******************************************************************************
PiezoDriver.h Takes Arduino Pin ID; Provides Knock Detection and Tune Generation

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

#ifndef __TUNE_PLAYER_H__
#define __TUNE_PLAYER_H__

//#include "PiezoDriver.h"
#include <stdint.h>
#include "IToneGen.h"
#include "IEventHandler.h"
#include "SigGen.h"
#include "SigScaler.h"
#include "Updater.h"

namespace SweetMaker
{

typedef enum
{
    SEMIBREVE = 8,
    MINIM = 4,
    CROTCHET = 2,
    QUAVER = 1
} NOTE_DURATION_8;

typedef enum
{
    ALLEGRETTO = 116,
    ALLEGRO = 144,
    VIVACE = 172
} TEMPO_BPM;

typedef struct
{
	uint16_t notePeriod_us;
	uint8_t duration_beats;
} NOTE;

#define NUM_NOTE(t) (sizeof(t)/sizeof(NOTE))

class TunePlayer: private AutoUpdate
{
public:
	TunePlayer();
	void configToneGen(IToneGen * toneGen);
	void configEventHandler(IEventHandler * eventHandler);
	void configEventHandler(IEventHandler::EventHandler eventHandler);
	void configReverb(uint8_t level, uint16_t period_ms);
	void update(uint16_t elapsedTime_ms);
	bool isRunning();

	void playTune(NOTE *tune, uint16_t numNotes, uint16_t tempo_bpm);
	void setTempo_bpm(uint16_t tempo_bpm); // beats per minute
	void stop(void);
	void mute();
	void unMute();
	
	typedef enum TunePlayerEvent{
		TUNE_ENDED = (IEventHandler::TUNE_PLAYER_EVENTS + 0),
		TUNE_STOPPED = (IEventHandler::TUNE_PLAYER_EVENTS + 1),
		TUNE_NEXT_NOTE = (IEventHandler::TUNE_PLAYER_EVENTS + 2)
	}TUNE_PLAYER_EVENT;

private:
    void dispatchEvent(uint16_t ref, TUNE_PLAYER_EVENT eventId);
	bool isMuted();
	void startTone(uint16_t note_us);
	SigGen sineGen;
	SigScaler sigScaler;

	uint16_t reverbLevel = 16;

	typedef enum
	{
	    IDLE = 0,
	    PLAYING_NOTE = 1,
	    DELAY_BETWEEN_NOTES = 2
	} PLAYER_STATE;

	IToneGen * toneGen;

	PLAYER_STATE playerState;
	uint16_t timeInState_ms;
	bool muted;

	NOTE * currentNotes;
	uint16_t numNotes;
	uint16_t currentNoteIndex;

	uint16_t currentNoteRemainingTime_ms;
	uint16_t beatDuration_ms;
	
	IEventHandler * eventHandlerInstance;
	IEventHandler::EventHandler eventHandlerCallback;
};

typedef enum
{
    NOTE_REST_US = 0,
    NOTE_C0_US = 61162,
    NOTE_CS0_US = 57737,
    NOTE_D0_US = 54496,
    NOTE_DS0_US = 51414,
    NOTE_E0_US = 48544,
    NOTE_F0_US = 45809,
    NOTE_FS0_US = 43253,
    NOTE_G0_US = 40816,
    NOTE_GS0_US = 38521,
    NOTE_A0_US = 36364,
    NOTE_AS0_US = 34317,
    NOTE_B0_US = 32394,
    NOTE_C1_US = 30581,
    NOTE_CS1_US = 28860,
    NOTE_D1_US = 27241,
    NOTE_DS1_US = 25714,
    NOTE_E1_US = 24272,
    NOTE_F1_US = 22910,
    NOTE_FS1_US = 21622,
    NOTE_G1_US = 20408,
    NOTE_GS1_US = 19264,
    NOTE_A1_US = 18182,
    NOTE_AS1_US = 17161,
    NOTE_B1_US = 16197,
    NOTE_C2_US = 15288,
    NOTE_CS2_US = 14430,
    NOTE_D2_US = 13620,
    NOTE_DS2_US = 12857,
    NOTE_E2_US = 12134,
    NOTE_F2_US = 11453,
    NOTE_FS2_US = 10811,
    NOTE_G2_US = 10204,
    NOTE_GS2_US = 9631,
    NOTE_A2_US = 9091,
    NOTE_AS2_US = 8581,
    NOTE_B2_US = 8099,
    NOTE_C3_US = 7645,
    NOTE_CS3_US = 7216,
    NOTE_D3_US = 6811,
    NOTE_DS3_US = 6428,
    NOTE_E3_US = 6068,
    NOTE_F3_US = 5727,
    NOTE_FS3_US = 5405,
    NOTE_G3_US = 5102,
    NOTE_GS3_US = 4816,
    NOTE_A3_US = 4545,
    NOTE_AS3_US = 4290,
    NOTE_B3_US = 4050,
    NOTE_C4_US = 3822,
    NOTE_CS4_US = 3608,
    NOTE_D4_US = 3405,
    NOTE_DS4_US = 3214,
    NOTE_E4_US = 3034,
    NOTE_F4_US = 2863,
    NOTE_FS4_US = 2703,
    NOTE_G4_US = 2551,
    NOTE_GS4_US = 2408,
    NOTE_A4_US = 2273,
    NOTE_AS4_US = 2145,
    NOTE_B4_US = 2025,
    NOTE_C5_US = 1911,
    NOTE_CS5_US = 1804,
    NOTE_D5_US = 1703,
    NOTE_DS5_US = 1607,
    NOTE_E5_US = 1517,
    NOTE_F5_US = 1432,
    NOTE_FS5_US = 1351,
    NOTE_G5_US = 1276,
    NOTE_GS5_US = 1204,
    NOTE_A5_US = 1136,
    NOTE_AS5_US = 1073,
    NOTE_B5_US = 1012,
    NOTE_C6_US = 956,
    NOTE_CS6_US = 902,
    NOTE_D6_US = 851,
    NOTE_DS6_US = 804,
    NOTE_E6_US = 758,
    NOTE_F6_US = 716,
    NOTE_FS6_US = 676,
    NOTE_G6_US = 638,
    NOTE_GS6_US = 602,
    NOTE_A6_US = 568,
    NOTE_AS6_US = 536,
    NOTE_B6_US = 506,
    NOTE_C7_US = 478,
    NOTE_CS7_US = 451,
    NOTE_D7_US = 426,
    NOTE_DS7_US = 402,
    NOTE_E7_US = 379,
    NOTE_F7_US = 358,
    NOTE_FS7_US = 338,
    NOTE_G7_US = 319,
    NOTE_GS7_US = 301,
    NOTE_A7_US = 284,
    NOTE_AS7_US = 268,
    NOTE_B7_US = 253,
    NOTE_C8_US = 239,
    NOTE_CS8_US = 225,
    NOTE_D8_US = 213,
    NOTE_DS8_US = 201,
    NOTE_E8_US = 190,
    NOTE_F8_US = 179,
    NOTE_FS8_US = 169,
    NOTE_G8_US = 159,
    NOTE_GS8_US = 150,
    NOTE_A8_US = 142,
    NOTE_AS8_US = 134,
    NOTE_B8_US = 127
} NOTE_PERIODS_US;


}

#endif

