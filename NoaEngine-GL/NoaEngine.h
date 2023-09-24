#ifndef NOAENGINE_GL_NOAENGINE_H
#define NOAENGINE_GL_NOAENGINE_H


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/glut.h>

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
#include <stdio.h>
#include <list>
#include <unordered_map>
#include <fstream>
#include <string>
#include <functional>

namespace noa {

	extern float deltaTime;

	extern int pixelWidth;
	extern int pixelHeight;

	extern uint32_t* pixelBuffer;
	#define DRAWPIXEL(x,y,color) ((x < 0 || y < 0 || x >= pixelWidth || y >= pixelHeight)?( - 1):(pixelBuffer[y * pixelWidth + x] = color))

	class NoaEngineGL
	{
	public:
		enum GameWindowMode
		{
			FullScreen = 0,
			WindowMode = 1
		};

	private:

		int glPixelWidth = 0;
		int glPixelHeight = 0;

		std::chrono::system_clock::time_point tp1 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime;
		std::chrono::system_clock::time_point tp2 = std::chrono::system_clock::now();

		GLFWwindow* window = nullptr;

		// 修正纹理坐标
		float vertices[16] = {
			// 顶点坐标        纹理坐标
			 1.0f,  1.0f,  1.0f, 1.0f, // 右上角
			 1.0f, -1.0f,  1.0f, 0.0f, // 右下角
			-1.0f, -1.0f,  0.0f, 0.0f, // 左下角
			-1.0f,  1.0f,  0.0f, 1.0f  // 左上角
		};
		unsigned int indices[6] = {
		   0, 1, 3, // 第一个三角形
		   1, 2, 3  // 第二个三角形
		};;

#pragma region SHADER

		const char* vertexShaderSource = R"glsl(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 0.0f, 1.0f);
    TexCoord = aTexCoord;
}
)glsl";

		const char* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
    FragColor = texture(ourTexture, TexCoord);
}
)glsl";

#pragma endregion

	public:
		NoaEngineGL(
			int width, int height,
			GameWindowMode windowMode,
			std::string gameName
		);

		virtual ~NoaEngineGL();

		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual void OnDisable() {};

		int Run();

	};

	extern int pixelWidth;
	extern int pixelHeight;

	extern void Debug(std::string msg);

}


#endif // !NOAENGINE_GL_NOAENGINE_H
