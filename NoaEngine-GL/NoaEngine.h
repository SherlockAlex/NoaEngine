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

using namespace std;

namespace noa {

	extern float deltaTime;

	class NoaEngine
	{
	public:
		enum GameWindowMode
		{
			FullScreen = 0,
			WindowMode = 1
		};

	private:

		uint32_t* pixelBuffer = nullptr;

		GLFWwindow* window = nullptr;

		int glPixelWidth = 0;
		int glPixelHeight = 0;

		std::chrono::system_clock::time_point tp1 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime;
		std::chrono::system_clock::time_point tp2 = chrono::system_clock::now();

	public:
		NoaEngine(
			int width, int height,
			GameWindowMode windowMode,
			string gameName
		);

		~NoaEngine();

		int Run();

	};

	extern void Debug(string msg);

}


#endif // !NOAENGINE_GL_NOAENGINE_H
