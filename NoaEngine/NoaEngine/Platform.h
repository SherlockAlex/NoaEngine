#ifndef NOAENGINE_PLATFORM
#define NOAENGINE_PLATFORM


#include <string>
#include <functional>
#include <chrono>

#include "SDLHelper.h"

#ifdef _WIN64

#include "GLTexture.h"
#include "GLRenderer.h"

#endif // _WIN64



namespace noa {

	//��������
	enum class WindowMode
	{
		FULLSCREEN = 1,
		WINDOW = 2048
	};

	//ƽ̨������
	class Platform {
	protected:
		//ϵͳʱ��

		uint32_t windowID = 0;

		bool isRun = true;

	public:

		Platform();
		virtual ~Platform();

		//��������
		virtual int Create(int width, int height,
				WindowMode windowMode,
				std::string gameName) = 0;

		//ȷ�����ڹر�
		virtual bool CheckWindowClose() = 0;

		void Quit();

		uint32_t GetWindowID();


	};

}


#endif

