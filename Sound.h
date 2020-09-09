#pragma once
#include "LibraryHeader.h"

#define SOUND_FILE_NAME "Sound/name.wav"
#define SOUND_FILE_NAME_BGM "Sound/bgm.wav"
#define SOUND_FILE_NAME_Bullet "Sound/bullet.wav"

class Sound
{
public:
	Sound();
	~Sound();
	void playSound_BGM();
};

