#include "Audio.h"

/// <summary>
/// Audio类的实现
/// </summary>
/// 
Audio::Audio(const char* filePath, AudioType type) {
	this->type = type;
	if (type == Music)
	{
		this->music = Mix_LoadMUS(filePath);
	}
	else if (type == Chunk)
	{
		this->chunk = Mix_LoadWAV(filePath);
	}
}

Audio::~Audio() {

}

void Audio::Play(bool loop) {
	if (type == Music)
	{
		if (loop)
		{
			Mix_PlayMusic(music, -1);
		}
		else {
			Mix_PlayMusic(music, 0);
		}
	}
	else if (type == Chunk)
	{
		if (loop)
		{
			Mix_PlayChannel(-1, chunk, -1);
		}
		else {
			Mix_PlayChannel(-1, chunk, 0);
		}
	}
}

void Audio::Stop()
{
	if (type == Music)
	{
		Mix_PauseMusic();
	}
	else if (type == Chunk)
	{
		Mix_HaltChannel(-1);
	}
}

