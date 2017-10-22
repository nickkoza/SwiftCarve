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

#include "AudioPlayer.h"

#include "SD.h"
#include "SPI.h"

AudioPlayer::AudioPlayer(int sdChipSelectPin, int speakerPin)
{
	if (SD.begin(sdChipSelectPin)) {
		Serial.println("Booting up audio");
		player = new TMRpcm();
		player->speakerPin = speakerPin;
	}
	else {
		player = NULL;
		Serial.println("Could not get SD card. Not going to play audio.");
	}
}

void AudioPlayer::play(char *file, boolean looped, boolean ifSilent)
{
    if (NULL == player || (ifSilent && player->isPlaying()))
    {
        return;
    }

    player->loop(looped);
    player->play(file);
}

void AudioPlayer::play(char *file)
{
    play(file, false, false);
}

void AudioPlayer::playLooped(char *file)
{
    play(file, true, false);
}

void AudioPlayer::playIfSilent(char *file)
{
    play(file, false, true);
}

void AudioPlayer::playLoopedIfSilent(char *file)
{
    play(file, true, true);
}

void AudioPlayer::stop()
{
    if (NULL == player)
    {
        return;
    }

    player->stopPlayback();
}