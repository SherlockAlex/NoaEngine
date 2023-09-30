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

	enum class WINDOWMODE
	{
		FULLSCREEN = 0,
		WINDOW = 1
	};

	class NoaEngine3D
	{
	public:
		

	private:

		int glPixelWidth = 0;
		int glPixelHeight = 0;

		std::chrono::system_clock::time_point tp1 = std::chrono::system_clock::now();
		std::chrono::system_clock::time_point tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;

		GLFWwindow* window = nullptr;

	public:
		NoaEngine3D(
			int width, int height,
			WINDOWMODE windowMode,
			std::string gameName
		);

		virtual ~NoaEngine3D();

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
