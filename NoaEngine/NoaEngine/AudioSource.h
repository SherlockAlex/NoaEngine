#ifndef NOAENGINE_AUDIO_H
#define NOAENGINE_AUDIO_H

#include <string>
#include <memory>
#include "ActorComponent.h"

namespace noa {

	class AudioClip;

	class AudioSource final :public ActorComponent
	{
	private:
		ACTOR_COMPONENT(AudioSource)
	private:
		std::shared_ptr<AudioClip> clip = nullptr;

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
	};
}

#endif