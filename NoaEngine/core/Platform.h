#ifndef NOAENGINE_PLATFORM
#define NOAENGINE_PLATFORM

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>
#include <string>
#include <functional>
#include <chrono>

#include "Graphic.h"

namespace noa {

	//ƽ̨������
	enum class WINDOWMODE
	{
		FULLSCREEN = 1,
		WINDOW = 2048
	};

	class Platform {
	protected:
		std::function<void()> engineUpdate;

		//ϵͳʱ��
		std::chrono::system_clock::time_point tp1 = std::chrono::system_clock::now();
		std::chrono::system_clock::time_point tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp1 - tp2;

		bool isRun = true;

	public:

		void SetEngineUpdate(std::function<void()> engineUpdate);

		//��������
		virtual int Create(int width, int height,
				WINDOWMODE windowMode,
				std::string gameName) = 0;

		//ȷ�����ڹر�
		virtual bool CheckWindowClose() = 0;

		virtual int SystemLoop() = 0;

		virtual int PresentWindow() = 0;

		void Quit();


	};



#ifdef _WIN64


	class Platform_OGL :public Platform {

	private:
		GLFWwindow* window;

		GLTexture* mainTexture;
		GLRenderer* mainRenderer;

	public:
		~Platform_OGL();

		int Create(int width, int height,
				WINDOWMODE windowMode,
				std::string gameName) override;

		bool CheckWindowClose() override;

		int SystemLoop() override;

		int PresentWindow() override;

	};

#endif // _WIN64

}


#endif

