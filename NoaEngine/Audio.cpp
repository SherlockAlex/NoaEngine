#include "Audio.h"

/// <summary>
/// Audio类的实现
/// </summary>

namespace noa {
	Audio::Audio(const char* filePath, AudioType type) {
#if (defined __linux) || (defined _WIN64)

		this->type = type;
		if (type == Music)
		{
			this->music = Mix_LoadMUS(filePath);
		}
		else if (type == Chunk)
		{
			this->chunk = Mix_LoadWAV(filePath);
		}
#endif // __linux
	}

	Audio::~Audio() {

	}

	void Audio::Play(bool loop) const
	{
#if (defined __linux) || (defined _WIN64)

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
#endif // __linux
	}

	void Audio::Stop() const
	{
#if (defined __linux) || (defined _WIN64)

		if (type == Music)
		{
			Mix_PauseMusic();
		}
		else if (type == Chunk)
		{
			Mix_HaltChannel(-1);
		}
#endif // __linux
	}


}

