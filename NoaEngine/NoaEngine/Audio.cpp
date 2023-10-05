#include "Audio.h"
#include "NoaEngine.h"

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
			Mix_VolumeChunk(this->chunk, static_cast<int>(MIX_MAX_VOLUME * volume));
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
			//Mix_VolumeMusic(static_cast<int>(MIX_MAX_VOLUME * volume));
		}
		else if (type == AudioType::CHUNK)
		{
			int channel = -1;
			if (loop)
			{
				channel = Mix_PlayChannelTimed(-1, chunk, -1,-1);
			}
			else {
				channel = Mix_PlayChannelTimed(-1, chunk, 0, -1); // 播放一次，持续时间设置为-1
			}
			//Mix_VolumeChunk(this->chunk, static_cast<int>(MIX_MAX_VOLUME * volume)); // 将音量设置为一半
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

