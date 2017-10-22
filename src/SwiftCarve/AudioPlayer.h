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