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
		NOBJECT(AudioSource)
	private:
		std::shared_ptr<AudioClip> clip = nullptr;

	private:
		AudioSource(Actor * actor);
		~AudioSource();

	public:

		static AudioSource* Create(Actor * actor);

		void SetVolume(float volume);
		void SetAudioClip(std::shared_ptr<AudioClip> clip);
		void Play(bool loop) const;
		void Stop() const;
	};
}

#endif