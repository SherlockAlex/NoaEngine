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

	//窗口类型
	enum class WindowMode
	{
		FULLSCREEN = 1,
		WINDOW = 2048
	};

	//平台抽象类
	class Platform {
	protected:
		//系统时钟

		uint32_t windowID = 0;

		bool isRun = true;

	public:

		Platform();
		virtual ~Platform();

		//创建窗口
		virtual int Create(int width, int height,
				WindowMode windowMode,
				std::string gameName) = 0;

		//确定窗口关闭
		virtual bool CheckWindowClose() = 0;

		void Quit();

		uint32_t GetWindowID();


	};

}


#endif

