#ifndef NOAENGINE_AUDIO_H
#define NOAENGINE_AUDIO_H

#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>

namespace noa {
	enum AudioType {
		Music,
		Chunk
	};

	class Audio {
	private:
		AudioType type = Music;
		Mix_Music* music = nullptr;
		Mix_Chunk* chunk = nullptr;
	public:
		//控制音量大小
		float volume = 1.0;

		Audio(const char* filePath, AudioType type);
		~Audio();
	public:
		void Play(bool loop) const;
		void Stop() const;
	};
}

#endif