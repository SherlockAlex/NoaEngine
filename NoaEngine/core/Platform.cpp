#include "Platform.h"
#include "NoaEngine.h"

namespace noa {

	extern std::shared_ptr<Renderer> renderer;

	Platform::Platform()
	{
		int init = SDL_Init(SDL_INIT_EVERYTHING);
		if (init != 0)
		{
			Debug("Game init failed");
			exit(-1);
		}
	}

	void Platform::Quit()
	{
		isRun = false;
	}

	SDL_Window* Platform::GetWindow()
	{
		return this->windows;
	}

#ifdef _WIN64



	//Platform_OGL::Platform_OGL():Platform()
	//{
	//	//初始化OpenGL

	//}

	//Platform_OGL::~Platform_OGL()
	//{
	//	delete mainTexture;
	//	glfwDestroyWindow(window);
	//	glfwTerminate();
	//}

	//int Platform_OGL::Create(int width, int height, WINDOWMODE windowMode, std::string gameName)
	//{
	//	if (!glfwInit()) {
	//		Debug("Failed to initialize GLFW");
	//		exit(-1);
	//	}
	//	//设置glfw窗口属性
	//	glfwWindowHint(GLFW_RESIZABLE, 0);
	//	GLFWmonitor* pMonitor = (windowMode == WINDOWMODE::FULLSCREEN) ? glfwGetPrimaryMonitor() : NULL;

	//	window = glfwCreateWindow(width, height, gameName.c_str(), pMonitor, NULL);
	//	if (!window) {
	//		Debug("Failed to create GLFW window");
	//		glfwTerminate();
	//		exit(-1);
	//	}

	//	glfwMakeContextCurrent(window);
	//	
	//	glfwSwapInterval(0);
	//	if (glewInit() != GLEW_OK) {
	//		Debug("Failed to initialize GLEW");
	//		exit(-1);
	//	}
	//	

	//	inputSystem.SetGraphicAPI(GRAPHIC::OPENGL);
	//	inputSystem.SetGLWindow(this->window);

	//	glfwSetScrollCallback(window, InputSystem::MouseScrollCallback);

	//	//初始化其余相关

	//	mainTexture = new GLTexture(width, height, pixelBuffer);

	//	return 0;
	//}

	//bool Platform_OGL::CheckWindowClose()
	//{

	//	return glfwWindowShouldClose(window) || !isRun;
	//}

	//int Platform_OGL::SystemLoop()
	//{

	//	tp2 = std::chrono::system_clock::now();
	//	elapsedTime = tp2 - tp1;
	//	Time::deltaTime = Time::timeScale * elapsedTime.count();

	//	Time::time += Time::deltaTime;
	//	if (Time::time > 2 * PI)
	//	{
	//		Time::time = 0;
	//	}

	//	// 执行游戏主类的update
	//	inputSystem.Update();

	//	int i = 0;
	//	renderer->Clear();
	//	mainTexture->UpdateTexture(pixelBuffer, pixelWidth, pixelHeight);
	//	renderer->DrawTexture(this->mainTexture,i, 0, 0, pixelWidth, pixelHeight);
	//	i++;
	//	for (const auto& instance : spriteInstancesGL)
	//	{
	//		renderer->DrawTexture(
	//			instance.texture
	//			,i
	//			, instance.position.x
	//			, instance.position.y
	//			, instance.scale.x
	//			, instance.scale.y
	//			, instance.eulerAngle
	//			, instance.flip
	//		);
	//		i++;
	//	}
	//	spriteInstancesGL.clear();

	//	glfwPollEvents();
	//	glfwSwapBuffers(window);
	//	tp1 = tp2;

	//	return 0;
	//}

	//int Platform_OGL::PresentWindow()
	//{
	//	glfwSwapBuffers(window);
	//	return 0;
	//}


#endif // _WIN64

	

	

}

