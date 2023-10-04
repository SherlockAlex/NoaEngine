#ifndef NOAENGINE_AUDIO_H
#define NOAENGINE_AUDIO_H

#include "SDLHelper.h"

namespace noa {
	enum class AudioType {
		MUSIC,
		CHUNK
	};

	class Audio {
	private:
		AudioType type = AudioType::MUSIC;
		Mix_Music* music = nullptr;
		Mix_Chunk* chunk = nullptr;
	public:
		float volume = 1.0;

		Audio(const char* filePath, AudioType type);
		~Audio();
	public:
		void Play(bool loop) const;
		void Stop() const;
	};
}

#endif