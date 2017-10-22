/*
Copyright(C) 2017 Nicholas Koza

This file is part of SwiftCarve.

This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _AUDIO_PLAYER_h
#define _AUDIO_PLAYER_h

#include "TMRpcm.h"

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "ArduinoMock.h"
#endif

class AudioPlayer
{
private:
	TMRpcm *player;
    void play(char *file, boolean looped, boolean ifSilent);

public:
	AudioPlayer(int sdChipSelectPin, int speakerPin);

	void play(char *file);
	void playLooped(char *file);
	void playIfSilent(char *file);
    void playLoopedIfSilent(char *file);
    void stop();
};

#endif