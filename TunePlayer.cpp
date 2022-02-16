/*******************************************************************************
TunePlayer.cpp

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


#include "TunePlayer.h"
using namespace SweetMaker;


#ifndef NULL
#define NULL 0
#endif

TunePlayer::TunePlayer()
{
	playerState = IDLE;
	timeInState_ms = 0;
	muted = false;

	currentNotes = NULL;
	currentNoteIndex = 0;
	currentNoteRemainingTime_ms = 0;

	beatDuration_ms = 0;
	setTempo_bpm(ALLEGRO);

	eventHandlerInstance = NULL;
	eventHandlerCallback = NULL;

	sineGen.configSamples(sineWave255, NUM_SAM(sineWave255), 150, 0);
	sineGen.configOutput(&sigScaler);

	sigScaler.configLinearScale(reverbLevel, 0);

}

void TunePlayer::configToneGen(IToneGen * gen)
{
	toneGen = gen;
	sigScaler.configOutput(gen);
}

void TunePlayer::configReverb(uint8_t level, uint16_t period_ms)
{
	reverbLevel = level;
	sigScaler.configLinearScale(reverbLevel, 0);
	sineGen.configPeriod_ms(period_ms);
}

void TunePlayer::configEventHandler(IEventHandler * handler)
{
	eventHandlerInstance = handler;
}

void TunePlayer::configEventHandler(IEventHandler::EventHandler handler)
{
	eventHandlerCallback = handler;
}

void TunePlayer::update(uint16_t elapsedTime_ms)
{
	timeInState_ms += elapsedTime_ms;

	if(playerState == IDLE)
		return;

	if (currentNoteRemainingTime_ms > elapsedTime_ms)
	{
		currentNoteRemainingTime_ms -= elapsedTime_ms;
		return;
	}

	if (playerState == PLAYING_NOTE)
	{
		sineGen.stop();
		toneGen->stop();
		currentNoteRemainingTime_ms = 50;
		playerState = DELAY_BETWEEN_NOTES;
		return;
	}

	if (++currentNoteIndex >= numNotes)
	{
		sineGen.stop();
		toneGen->stop();
		this->timeInState_ms = 0;
		playerState = IDLE;
		dispatchEvent(0, TUNE_ENDED);
		return;
	}

	currentNoteRemainingTime_ms = currentNotes[currentNoteIndex].duration_beats * beatDuration_ms;
	if (currentNotes[currentNoteIndex].notePeriod_us == NOTE_REST_US)
	{
		sineGen.stop();
		toneGen->stop();
	}
	else
	{
		uint16_t note_period_us = currentNotes[currentNoteIndex].notePeriod_us;
		startTone(note_period_us);
		dispatchEvent(currentNoteIndex, TUNE_NEXT_NOTE);
	}
	playerState = PLAYING_NOTE;
}

void TunePlayer::playTune(NOTE * notes, uint16_t num, uint16_t tempo)
{
	numNotes = num;
	currentNoteIndex = 0;
	currentNotes = notes;

	setTempo_bpm(tempo);

	currentNoteRemainingTime_ms = currentNotes[currentNoteIndex].duration_beats * beatDuration_ms;
	if (currentNotes[currentNoteIndex].notePeriod_us == NOTE_REST_US)
	{
		mute();
	}
	else
	{
		uint16_t note_period_us = currentNotes[currentNoteIndex].notePeriod_us;
		startTone(note_period_us);
	}
	playerState = PLAYING_NOTE;
	timeInState_ms = 0;

}

bool TunePlayer::isRunning()
{
	if (playerState != IDLE)
		return (true);
	return (false);
}

void TunePlayer::setTempo_bpm(uint16_t tempo_bpm)
{
	if(tempo_bpm == 0)
		return;
	unsigned long bd_ms = 60000 / tempo_bpm;
	beatDuration_ms = (uint16_t)bd_ms;
}

void TunePlayer::stop(void)
{
	sineGen.stop();
	toneGen->stop();
	this->timeInState_ms = 0;
	playerState = IDLE;
	dispatchEvent(0, TUNE_STOPPED);
}


void TunePlayer::dispatchEvent(uint16_t eventInfo, TUNE_PLAYER_EVENT eventId)
{
	if (eventHandlerCallback)
		eventHandlerCallback(eventId, 0, eventInfo);

	if (eventHandlerInstance)
		eventHandlerInstance->handleEvent(eventId, 0, eventInfo);
}

void TunePlayer::mute()
{
	sineGen.stop();
	toneGen->stop();
	muted = true;
}

void TunePlayer::unMute()
{
	if (playerState == PLAYING_NOTE)
	{
		uint16_t currentNote = currentNotes[currentNoteIndex].notePeriod_us;
		startTone(currentNote);
		sineGen.start();
	}
	muted = false;
}

bool TunePlayer::isMuted()
{
	return muted;
}

void TunePlayer::startTone(uint16_t note_us)
{
	sigScaler.configLinearScale(reverbLevel, note_us);
	if (isMuted())
		return;

	sineGen.start();
}