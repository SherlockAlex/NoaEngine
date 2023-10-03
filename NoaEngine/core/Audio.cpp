#include "Audio.h"
#include "NoaEngine.h"
/// <summary>
/// Audio类的实现
/// </summary>

namespace noa {
	Audio::Audio(const char* filePath, AudioType type) {

		this->type = type;
		if (type == AudioType::MUSIC)
		{
			this->music = Mix_LoadMUS(filePath);
			Mix_VolumeMusic(static_cast<int>(MIX_MAX_VOLUME * volume));
		}
		else if (type == AudioType::CHUNK)
		{
			this->chunk = Mix_LoadWAV(filePath);
			Mix_VolumeChunk(this->chunk, static_cast<int>(MIX_MAX_VOLUME * volume)); // 将音量设置为一半
		}
	}

	Audio::~Audio() {
		if (chunk!=nullptr)
		{
			Mix_FreeChunk(chunk);
		}
		if (music!=nullptr)
		{
			Mix_FreeMusic(music);
		}
		Debug("Remove audio resource successfully");
	}

	void Audio::Play(bool loop) const
	{
		if (type == AudioType::MUSIC)
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
			if (loop)
			{
				Mix_PlayChannel(-1, chunk, -1);
			}
			else {
				Mix_PlayChannel(-1, chunk, 0);
			}
			Mix_VolumeChunk(this->chunk, static_cast<int>(MIX_MAX_VOLUME * volume)); // 将音量设置为一半
		}
	}

	void Audio::Stop() const
	{
		if (type == AudioType::MUSIC)
		{
			Mix_PauseMusic();
		}
		else if (type == AudioType::CHUNK)
		{
			Mix_HaltChannel(-1);
		}
	}


}

