#include "Platform.h"
#include "NoaEngine.h"

namespace noa {

	void Platform::SetEngineUpdate(std::function<void()> engineUpdate)
	{
		this->engineUpdate = engineUpdate;
	}

	void Platform::Quit()
	{
		isRun = false;
	}

#ifdef _WIN64



	Platform_OGL::~Platform_OGL()
	{
		delete mainTexture;
		delete mainRenderer;
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	int Platform_OGL::Create(int width, int height, WINDOWMODE windowMode, std::string gameName)
	{

		//初始化OpenGL
		if (!glfwInit()) {
			Debug("Failed to initialize GLFW");
			exit(-1);
		}

		//设置glfw窗口属性
		glfwWindowHint(GLFW_RESIZABLE, 0);
		GLFWmonitor* pMonitor = (windowMode == WINDOWMODE::FULLSCREEN) ? glfwGetPrimaryMonitor() : NULL;

		window = glfwCreateWindow(width, height, gameName.c_str(), pMonitor, NULL);
		if (!window) {
			Debug("Failed to create GLFW window");
			glfwTerminate();
			exit(-1);
		}

		glfwMakeContextCurrent(window);
		glfwSwapInterval(0);

		if (glewInit() != GLEW_OK) {
			Debug("Failed to initialize GLEW");
			exit(-1);
		}

		inputSystem.SetGraphicAPI(GRAPHIC::OPENGL);
		inputSystem.SetGLWindow(this->window);

		glfwSetScrollCallback(window, InputSystem::MouseScrollCallback);

		//初始化其余相关
		renderer.API = GRAPHIC::OPENGL;

		mainTexture = new GLTexture(width, height, pixelBuffer);
		mainRenderer = new GLRenderer();

		return 0;
	}

	bool Platform_OGL::CheckWindowClose()
	{
		return glfwWindowShouldClose(window);
	}

	int Platform_OGL::SystemLoop()
	{

		while ((!glfwWindowShouldClose(window)) && isRun) {
			tp2 = std::chrono::system_clock::now();
			elapsedTime = tp2 - tp1;
			Time::deltaTime = Time::timeScale * elapsedTime.count();

			Time::time += Time::deltaTime;
			if (Time::time > 2 * PI)
			{
				Time::time = 0;
			}

			// 执行游戏主类的update
			inputSystem.Update();
			sceneManager.Update();
			this->engineUpdate();

			glClear(GL_COLOR_BUFFER_BIT);  // 清空颜色缓冲区和深度缓冲区
			int i = 0;
			glActiveTexture(GL_TEXTURE + i);
			mainTexture->UpdateTexture(pixelBuffer, pixelWidth, pixelHeight);
			mainRenderer->DrawTexture(this->mainTexture, 0, 0, pixelWidth, pixelHeight);
			i++;
			for (const auto& instance : spriteInstancesGL)
			{
				glActiveTexture(GL_TEXTURE + i);
				mainRenderer->DrawTexture(
					instance.texture
					, instance.position.x
					, instance.position.y
					, instance.scale.x
					, instance.scale.y
					, instance.eulerAngle
					, instance.flip
				);
				i++;
			}


			glfwPollEvents();
			glfwSwapBuffers(window);
			spriteInstancesGL.clear();

			tp1 = tp2;
		}

		return 0;
	}

	int Platform_OGL::PresentWindow()
	{
		glfwSwapBuffers(window);
		return 0;
	}

#endif // _WIN64

	

}

