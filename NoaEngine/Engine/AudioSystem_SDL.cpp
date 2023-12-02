#if defined(NOA_WINDOWS)

/**
* 初始化Windows平台的声音系统
*/

#include "AudioSystem_SDL.h"
#include "Debug.h"

void noa::AudioSystem_SDL::InitAudioSubSystem()
{
	if (Mix_OpenAudio(
		MIX_DEFAULT_FREQUENCY,
		MIX_DEFAULT_FORMAT,
		MIX_CHANNELS,
		4096
	) == -1)
	{
		AudioSystem::init = false;
	}
	AudioSystem::init = true;
}

noa::AudioSystem_SDL::~AudioSystem_SDL() {
	Mix_CloseAudio();
}

std::shared_ptr<noa::AudioClip> noa::AudioSystem_SDL::LoadAudioClip(const std::string& filename)
{
	std::shared_ptr<noa::AudioClip> clip = std::make_shared<noa::AudioClip_SDL>();
	clip->LoadAudioSource(filename);
	return clip;
}

noa::AudioClip_SDL::~AudioClip_SDL()
{
	if (!AudioSystem::init)
	{
		return;
	}

	Mix_FreeMusic(music);
}

void noa::AudioClip_SDL::LoadAudioSource(const std::string& filename)
{
	this->music = Mix_LoadMUS(filename.c_str());
	if (this->music == nullptr)
	{
		noa::Debug::Log("Audio clip is null:"+filename);
		return;
	}
	
}

void noa::AudioClip_SDL::Play(bool isLoop)
{
	if (this->music == nullptr)
	{
		return;
	}
	if (isLoop)
	{
		Mix_PlayMusic(music, -1);
	}
	else {
		Mix_PlayMusic(music, 0);
	}
	
}

void noa::AudioClip_SDL::Stop()
{
	Mix_PauseMusic();
}


#endif

