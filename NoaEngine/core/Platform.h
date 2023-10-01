#ifndef NOAENGINE_PLATFORM
#define NOAENGINE_PLATFORM


/*
* Ϊ�˸��õ�֧����Ϸ����Ŀ�ƽ̨���ԣ����ǲ�����OpenGL��Ϊ����ƽ̨��ͼ�νӿڣ�������OpenGLES��Ϊ�ƶ��豸�˵�ͼ�νӿ�(Ŀǰ���ڿ�����)��
* ƽ̨���棬���Ǿ���ͳһʹ��SDL2���ڴ������ڡ�
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

	//��������
	enum class WindowMode
	{
		FULLSCREEN = 1,
		WINDOW = 2048
	};

	//ƽ̨������
	class Platform {
	protected:

		SDL_Window* windows = nullptr;

		//ϵͳʱ��

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

