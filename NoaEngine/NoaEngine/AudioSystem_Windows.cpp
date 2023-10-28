#if defined(_WIN64)||defined(_WIN32)

/**
* 初始化Windows平台的声音系统
*/

#include "AudioSystem_Windows.h"
#include "Debug.h"

void noa::AudioSystem_Windows::InitAudioSubSystem()
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

std::shared_ptr<noa::AudioClip> noa::AudioSystem_Windows::LoadAudioClip(const std::string& filename)
{
	std::shared_ptr<noa::AudioClip> clip = std::make_shared<noa::AudioClip_Windows>();
	clip->LoadAudioSource(filename);
	return clip;
}

noa::AudioClip_Windows::~AudioClip_Windows()
{
	if (!AudioSystem::init)
	{
		return;
	}

	Mix_FreeMusic(music);
}

void noa::AudioClip_Windows::LoadAudioSource(const std::string& filename)
{
	this->music = Mix_LoadMUS(filename.c_str());
	if (this->music == nullptr)
	{
		noa::Debug::Log("Audio clip is null:"+filename);
		return;
	}
	
}

void noa::AudioClip_Windows::Play(bool isLoop)
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
	//Mix_VolumeMusic(static_cast<int>(MIX_MAX_VOLUME * volume));
}

void noa::AudioClip_Windows::Stop()
{
	Mix_PauseMusic();
}


#endif

