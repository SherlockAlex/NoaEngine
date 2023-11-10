#include "AudioSource.h"
#include "AudioSystem.h"

noa::AudioSource::AudioSource(Actor * actor):ActorComponent(actor) {

}

noa::AudioSource::~AudioSource() 
{
	
}

noa::AudioSource* noa::AudioSource::Create(Actor* actor)
{
	return NObject<AudioSource>::Create(actor);
}

noa::AudioSource& noa::AudioSource::SetVolume(float volume)
{
	if (this->clip == nullptr)
	{
		return *this;
	}
	this->clip->SetVolume(volume);
	return *this;
}

noa::AudioSource& noa::AudioSource::SetAudioClip(std::shared_ptr<AudioClip> clip)
{
	this->clip = clip;
	return *this;
}

noa::AudioSource * noa::AudioSource::Apply()
{
	return this;
}

void noa::AudioSource::Play(bool loop) const
{
	if (!AudioSystem::init||this->clip == nullptr)
	{
		return;
	}

	this->clip->Play(loop);
	
}

void noa::AudioSource::Stop() const
{

	if (!AudioSystem::init||this->clip == nullptr)
	{
		return;
	}

	this->clip->Stop();

}


