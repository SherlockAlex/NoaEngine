#include <ctime>

#include "NoaEngine.h"

namespace noa {

	float deltaTime = 0;

	NoaEngine::NoaEngine(int width, int height, GameWindowMode windowMode, string gameName)
	{
		//初始化
		glfwInit();

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		//创建窗口
		window = glfwCreateWindow(width, height, gameName.c_str(), nullptr, nullptr);
		if (window == nullptr)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			exit(0);
			//return -1;
		}

		glfwMakeContextCurrent(window);

		glewExperimental = GL_TRUE;

		if (glewInit() != GLEW_OK)
		{
			std::cout << "Failed to initialize GLEW" << std::endl;
			exit(0);
		}

		glfwGetFramebufferSize(window, &glPixelWidth, &glPixelHeight);
		glViewport(0, 0, glPixelWidth, glPixelHeight);

		pixelBuffer = new uint32_t[glPixelWidth * glPixelHeight];

	}

	NoaEngine::~NoaEngine()
	{
		//释放资源
		glfwTerminate();
	}

	int NoaEngine::Run()
	{

		tp1 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime;
		tp2 = chrono::system_clock::now();

		while (!glfwWindowShouldClose(window))
		{
			tp2 = chrono::system_clock::now();
			elapsedTime = tp2 - tp1;
			deltaTime = elapsedTime.count();
			
			glfwPollEvents();
			glfwSwapBuffers(window);

			Debug(to_string(1.0/deltaTime));

			for (int x = 0;x<glPixelWidth;x++) 
			{
				for (int y = 0;y<glPixelHeight;y++)
				{
					const uint32_t color = 0;
					pixelBuffer[y * glPixelWidth + x] = color;
				}
			}

			glDrawPixels(glPixelWidth, glPixelHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);

			// 交换缓冲区，显示绘制结果
			glfwSwapBuffers(window);

			tp1 = tp2;

		}

		return 0;

	}

	void Debug(string msg)
	{
		//下面显示时间的部分的代码可能编译会出现异常，注意在编译的预处理请加上 _CRT_SECURE_NO_WARNINGS
		std::time_t current_time = std::time(nullptr);

		// 将当前时间转换为字符串格式
		char time_string[100];
		std::strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", std::localtime(&current_time));
		cout << "[INFO " << time_string << "]:" << msg << endl;
	}

}

