#ifndef NOAENGINE_AUDIO_H
#define NOAENGINE_AUDIO_H

#include <string>
#include <memory>
#include "ActorComponent.h"
#include "Core.h"

namespace noa {

	class NOA_API AudioClip;

	class NOA_API AudioSource final 
		:public ActorComponent
	{
	private:
		ACTOR_COMPONENT(AudioSource)
	private:
		AudioSource(Actor * actor);
		~AudioSource();

	public:

		static AudioSource* Create(Actor * actor);

		AudioSource& SetVolume(float volume);
		AudioSource& SetAudioClip(std::shared_ptr<AudioClip> clip);
		AudioSource* Apply();

		void Play(bool loop) const;
		void Stop() const;

	private:
		std::shared_ptr<AudioClip> clip = nullptr;

	};
}

#endif