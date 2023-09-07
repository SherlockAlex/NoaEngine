#include "Audio.h"

/// <summary>
/// Audio���ʵ��
/// </summary>

namespace noa {
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

