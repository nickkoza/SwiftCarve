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