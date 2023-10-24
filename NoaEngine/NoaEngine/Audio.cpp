#include "Audio.h"

bool noa::Audio::isInitSuccessful = false;

noa::Audio::Audio(const char* filePath, noa::AudioType type) {
	
	if (!isInitSuccessful) 
	{
		return;
	}

	this->type = type;

	switch (type)
	{
	case noa::AudioType::MUSIC:
		this->music = Mix_LoadMUS(filePath);
		Mix_VolumeMusic(static_cast<int>(MIX_MAX_VOLUME * volume));
		break;
	case noa::AudioType::CHUNK:
		this->chunk = Mix_LoadWAV(filePath);
		Mix_VolumeChunk(this->chunk, static_cast<int>(MIX_MAX_VOLUME * volume));
		break;
	default:
		break;
	}

}

noa::Audio::~Audio() {
	if (!isInitSuccessful)
	{
		return;
	}

	if (chunk != nullptr)
	{
		Mix_FreeChunk(chunk);
	}

	if (music != nullptr)
	{
		Mix_FreeMusic(music);
	}
}

void noa::Audio::Play(bool loop) const
{
	if (!isInitSuccessful)
	{
		return;
	}

	if (type == noa::AudioType::MUSIC)
	{
		if (loop)
		{
			Mix_PlayMusic(music, -1);
		}
		else {
			Mix_PlayMusic(music, 0);
		}
		Mix_VolumeMusic(static_cast<int>(MIX_MAX_VOLUME * volume));
	}
	else if (type == AudioType::CHUNK)
	{
		int channel = -1;
		if (loop)
		{
			channel = Mix_PlayChannelTimed(-1, chunk, -1, -1);
		}
		else {
			channel = Mix_PlayChannelTimed(-1, chunk, 0, -1); // 播放一次，持续时间设置为-1
		}
		Mix_VolumeChunk(this->chunk, static_cast<int>(MIX_MAX_VOLUME * volume)); // 将音量设置为一半
	}
}

void noa::Audio::Stop() const
{

	if (!isInitSuccessful)
	{
		return;
	}

	switch (type)
	{
	case noa::AudioType::MUSIC:
		Mix_PauseMusic();
		break;
	case noa::AudioType::CHUNK:
		Mix_HaltChannel(-1);
		break;
	default:
		break;
	}

}


