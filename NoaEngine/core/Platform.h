#ifndef NOAENGINE_PLATFORM
#define NOAENGINE_PLATFORM


/*
* 为了更好的支持游戏引擎的跨平台特性，我们采用了OpenGL作为主机平台的图形接口，并采用OpenGLES作为移动设备端的图形接口(目前正在开发中)，
* 平台方面，我们决定统一使用SDL2用于创建窗口。
*/
#include <string>
#include <functional>
#include <chrono>

#include <SDL2/SDL.h>

#include "Graphic.h"

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

		SDL_Window* windows = nullptr;

		//系统时钟

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

		SDL_Window* GetWindow();


	};



#ifdef _WIN64

	//class Platform_OGL :public Platform {

	//private:
	//	GLFWwindow* window;

	//	GLTexture* mainTexture;
	//	//GLRenderer* mainRenderer;

	//public:
	//	Platform_OGL();
	//	~Platform_OGL();

	//	int Create(int width, int height,
	//			WINDOWMODE windowMode,
	//			std::string gameName) override;

	//	bool CheckWindowClose() override;

	//	int SystemLoop() override;

	//	int PresentWindow() override;

	//};

#endif // _WIN64

}


#endif

