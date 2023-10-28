#if defined(_WIN64) ||defined(_WIN32)

#ifndef NOAENGINE_AUDIOSYSTEM_WINDOWS_H
#define NOAENGINE_AUDIOSYSTEM_WINDOWS_H

#include "AudioSystem.h"
#include "SDLHelper.h"


namespace noa {

	class AudioClip_Windows:public AudioClip {
		//ƽ̨������Դ����
	private:
		Mix_Music* music = nullptr;
	public:
		~AudioClip_Windows();
		void Play(bool isLoop) override;
		void Stop() override;
		void LoadAudioSource(const std::string& filename) override;

	};

	class AudioSystem_Windows :
		public AudioSystem
	{
	public:
		void InitAudioSubSystem() override;
		std::shared_ptr<AudioClip> LoadAudioClip(const std::string& filename) override;
	};

}

#endif // !NOAENGINE_AUDIOSYSTEM_WINDOWS_H

#endif //_WIN64
