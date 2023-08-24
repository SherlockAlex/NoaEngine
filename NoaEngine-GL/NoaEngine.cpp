#include <ctime>

#include "NoaEngine.h"

namespace noa {

	float deltaTime = 0;

	NoaEngine::NoaEngine(int width, int height, GameWindowMode windowMode, string gameName)
	{

		this->glPixelHeight = height;
		this->glPixelWidth = width;

		pixelBuffer = new uint32_t[width * height];

		// ��ʼ��GLFW��
		if (!glfwInit()) {
			exit(0);
			//return -1;
		}

		// ��������
		window = glfwCreateWindow(width, height, gameName.c_str(), NULL, NULL);
		if (!window) {
			glfwTerminate();
			exit(0);
		}

		glfwMakeContextCurrent(window);

		// ��ʼ��GLEW��
		if (glewInit() != GLEW_OK) {
			glfwTerminate();
			exit(0);
		}

		// �����ӿڴ�С
		glViewport(0, 0, width, height);

		// �����������
		
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		// �����������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// �ϴ��������ݵ�����
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);

		////��ʼ��
		//glfwInit();

		//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		////��������
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
		//�ͷ���Դ
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

			// �����ɫ����
			//glClear(GL_COLOR_BUFFER_BIT);

			// ��Ⱦ����
			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
			glEnd();

			// ����ǰ�󻺳�
			glfwSwapBuffers(window);

			// �����¼�
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
			//// ��������������ʾ���ƽ��
			//glfwSwapBuffers(window);

			tp1 = tp2;

		}

		return 0;

	}

	void Debug(string msg)
	{
		//������ʾʱ��Ĳ��ֵĴ�����ܱ��������쳣��ע���ڱ����Ԥ��������� _CRT_SECURE_NO_WARNINGS
		std::time_t current_time = std::time(nullptr);

		// ����ǰʱ��ת��Ϊ�ַ�����ʽ
		char time_string[100];
		std::strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", std::localtime(&current_time));
		cout << "[INFO " << time_string << "]:" << msg << endl;
	}

}

