#include <ctime>

#include "NoaEngine.h"

namespace noa {

	float deltaTime = 0;

	NoaEngine::NoaEngine(int width, int height, GameWindowMode windowMode, string gameName)
	{

		this->glPixelHeight = height;
		this->glPixelWidth = width;

		pixelBuffer = new uint32_t[width * height];

		// 初始化GLFW库
		if (!glfwInit()) {
			exit(0);
			//return -1;
		}

		// 创建窗口
		window = glfwCreateWindow(width, height, gameName.c_str(), NULL, NULL);
		if (!window) {
			glfwTerminate();
			exit(0);
		}

		glfwMakeContextCurrent(window);

		// 初始化GLEW库
		if (glewInit() != GLEW_OK) {
			glfwTerminate();
			exit(0);
		}

		// 设置视口大小
		glViewport(0, 0, width, height);

		// 创建纹理对象
		
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// 设置纹理参数
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// 上传像素数据到纹理
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);

		////初始化
		//glfwInit();

		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		////创建窗口
		//window = glfwCreateWindow(width, height, gameName.c_str(), nullptr, nullptr);
		//if (window == nullptr)
		//{
		//	std::cout << "Failed to create GLFW window" << std::endl;
		//	glfwTerminate();
		//	exit(0);
		//	//return -1;
		//}

		//glfwMakeContextCurrent(window);

		//glewExperimental = GL_TRUE;

		//if (glewInit() != GLEW_OK)
		//{
		//	std::cout << "Failed to initialize GLEW" << std::endl;
		//	exit(0);
		//}

		//glfwGetFramebufferSize(window, &glPixelWidth, &glPixelHeight);
		//glViewport(0, 0, glPixelWidth, glPixelHeight);

		//pixelBuffer = new uint32_t[glPixelWidth * glPixelHeight];

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
			
			for (int x = 0;x<glPixelWidth;x++) 
			{
				for (int y = 0; y < glPixelHeight;y++) {
					pixelBuffer[y * glPixelWidth + x] = 128;
				}
			}

			// 清空颜色缓冲
			//glClear(GL_COLOR_BUFFER_BIT);

			// 渲染矩形
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
			glEnd();

			// 交换前后缓冲
			glfwSwapBuffers(window);

			// 处理事件
			glfwPollEvents();

			//glfwPollEvents();
			//glfwSwapBuffers(window);

			//

			//for (int x = 0;x<glPixelWidth;x++) 
			//{
			//	for (int y = 0;y<glPixelHeight;y++)
			//	{
			//		const uint32_t color = 255;
			//		pixelBuffer[y * glPixelWidth + x] = color;
			//	}
			//}

			//glDrawPixels(glPixelWidth, glPixelHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);
			//
			//// 交换缓冲区，显示绘制结果
			//glfwSwapBuffers(window);

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

