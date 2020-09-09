#include "Sound.h"

Sound::Sound()
{
}

Sound::~Sound()
{
}

void Sound::playSound_BGM()
{
	PlaySound(TEXT(SOUND_FILE_NAME_BGM), NULL, SND_ASYNC | SND_LOOP);
	printf("SoundFile Loaded\n");
}