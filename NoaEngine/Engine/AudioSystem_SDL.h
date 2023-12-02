#if defined(_WIN64) ||defined(_WIN32)

#ifndef NOAENGINE_AUDIOSYSTEM_WINDOWS_H
#define NOAENGINE_AUDIOSYSTEM_WINDOWS_H

#include "AudioSystem.h"
#include "SDLHelper.h"
#include "Core.h"

namespace noa {

	class NOA_API AudioClip_SDL:public AudioClip {
		//平台声音资源抽象
	private:
		Mix_Music* music = nullptr;
	public:
		~AudioClip_SDL();
		void Play(bool isLoop) override;
		void Stop() override;
		void LoadAudioSource(const std::string& filename) override;

	};

	class NOA_API AudioSystem_SDL :
		public AudioSystem
	{
	public:
		void InitAudioSubSystem() override;
		std::shared_ptr<AudioClip> LoadAudioClip(const std::string& filename) override;
		virtual ~AudioSystem_SDL();
	};

}

#endif // !NOAENGINE_AUDIOSYSTEM_WINDOWS_H

#endif //_WIN64
