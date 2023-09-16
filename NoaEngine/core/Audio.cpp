#include "Audio.h"

/// <summary>
/// Audio类的实现
/// </summary>

namespace noa {
	Audio::Audio(const char* filePath, AudioType type) {
		this->type = type;
		if (type == Music)
		{
			this->music = Mix_LoadMUS(filePath);
			Mix_VolumeMusic(MIX_MAX_VOLUME*volume);
		}
		else if (type == Chunk)
		{
			this->chunk = Mix_LoadWAV(filePath);
			Mix_VolumeChunk(this->chunk, MIX_MAX_VOLUME*volume); // 将音量设置为一半
		}
	}

	Audio::~Audio() {
		Mix_CloseAudio();
	}

	void Audio::Play(bool loop) const
	{
		if (type == Music)
		{
			if (loop)
			{
				Mix_PlayMusic(music, -1);
			}
			else {
				Mix_PlayMusic(music, 0);
			}
			Mix_VolumeMusic(MIX_MAX_VOLUME * volume);
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
			Mix_VolumeChunk(this->chunk, MIX_MAX_VOLUME * volume); // 将音量设置为一半
		}
	}

	void Audio::Stop() const
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


}

