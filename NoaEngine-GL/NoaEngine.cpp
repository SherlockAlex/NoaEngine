#include <ctime>
#include "NoaEngine.h"

using namespace std;

namespace noa {

	float deltaTime = 0;
    int pixelWidth = 0;
    int pixelHeight = 0;

    uint32_t* pixelBuffer = nullptr;

	NoaEngineGL::NoaEngineGL(int width, int height, GameWindowMode windowMode, string gameName)
	{

        if (!glfwInit()) {
            Debug("Failed to initialize GLFW");
            exit(-1);
        }

		this->glPixelHeight = height;
		this->glPixelWidth = width;

        pixelWidth = width;
        pixelHeight = height;

		pixelBuffer = new uint32_t[width * height];

        window = glfwCreateWindow(width, height, gameName.c_str(), NULL, NULL);
        if (!window) {
            Debug("Failed to create GLFW window");
            glfwTerminate();
            exit(-1);
        }

        glfwMakeContextCurrent(window);

        if (glewInit() != GLEW_OK) {
            Debug("Failed to initialize GLEW");
            exit(-1);
        }

        
		

	}

	NoaEngineGL::~NoaEngineGL()
	{
		
        glfwDestroyWindow(window);
        glfwTerminate();
	}

	int NoaEngineGL::Run()
	{

        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        GLuint shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        GLuint VBO, VAO, EBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glPixelWidth, glPixelHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);


        Start();

        while (!glfwWindowShouldClose(window)) {
            tp2 = std::chrono::system_clock::now();
            elapsedTime = tp2 - tp1;
            deltaTime = elapsedTime.count();

            glClear(GL_COLOR_BUFFER_BIT);

            ///修改像素值

            Update();

            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->glPixelWidth, this->glPixelHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);

            ///

            glUseProgram(shaderProgram);
            glBindTexture(GL_TEXTURE_2D, textureID);
            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

            glfwSwapBuffers(window);
            glfwPollEvents();

            tp1 = tp2;
        }

        //释放资源
        glDeleteProgram(shaderProgram);
        glDeleteTextures(1, &textureID);
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);

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

