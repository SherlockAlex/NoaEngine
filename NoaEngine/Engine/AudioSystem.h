#ifndef NOAENGINE_AUDIOSYSTEM_H
#define NOAENGINE_AUDIOSYSTEM_H

#include <memory>
#include <string>

#include "Core.h"

namespace noa {

	class NOA_API AudioClip {
		//ƽ̨������Դ����
	protected:
		float volume = 1.0;
	public:
		virtual void LoadAudioSource(const std::string& filename) = 0;
		virtual void Play(bool isLoop) = 0;
		virtual void Stop() = 0;
		void SetVolume(float value);
	};

	//�����࣬�������ƽ̨����ϵͳ�ĳ���
	class NOA_API AudioSystem
	{
	public:
		static bool init;
	public:
		virtual void InitAudioSubSystem() = 0;
		virtual std::shared_ptr<AudioClip> LoadAudioClip(const std::string & filename) = 0;
	};

	extern std::shared_ptr<AudioSystem> audioSystem;
	
}



#endif // !NOAENGINE_AUDIOSYSTEM_H

