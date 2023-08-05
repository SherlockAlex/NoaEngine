#ifndef NOAENGINE_AUDIO_H
#define NOAENGINE_AUDIO_H

#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>

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
	Audio(const char* filePath, AudioType type);
	~Audio();
public:
	void Play(bool loop);
};

#endif