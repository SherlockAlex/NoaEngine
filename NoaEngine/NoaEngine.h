/*
* NoaEngine游戏引擎内容如下:
* Animator					动画器
* Audio						声音模块
* Behaviour					脚本模块
* GameObject				游戏物品
* InputSystem				输入系统
* NoaEngine					游戏基类
* NoaMath					常用的数学模块
* Renderer					渲染器，用于绘制画面
* Scene						场景地图模块
* Sprite					精灵模块
* 
* 其中还有一个游戏例子d_*开头的项目
* 
* 如果你是Linux用户，请仔细阅读Makefile文件，你可以使用它快速的配置开发环境
* 如果你是Windows用户，请考虑使用VS 2022进行开发，注意配置好SDL2的开发环境
* 
*/

/*
	//a game example:

	#include "NoaEngine.h"

	using namespace noa;

	class ExampleGame :public NoaGameEngine {
	public:
		ExampleGame(int width, int height,
			GameWindowMode windowMode,
			string gameName) :NoaGameEngine(width,height,windowMode,gameName) 
		{
			//game inintialize
		}

	public:

		void Start() override 
		{
			//called once when game start
		}

		void Update() override 
		{
			//called when each frame
		}


	};

	int main(int argc, char* argv[])
	{
		ExampleGame game(1920,1080,NoaGameEngine::FullScreen,"ExampleGame");
		return game.Run();
	}

*/


#ifndef NOAENGINE_H
#define NOAENGINE_H

#define SDL_MAIN_HANDLED

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

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_thread.h>

#include <dwmapi.h>
#include <GL/gl.h>

#include "NoaMath.h"
#include "Scene.h"
#include "Audio.h"
#include "InputSystem.h"
#include "Renderer.h"
#include "Behaviour.h"
#include "GameObject.h"
#include "Animator.h"
#include "Sprite.h"
#include "Physics.h"
#include "NoaAction.h"

//窗口属性

namespace noa {
	extern int pixelHeight;
	extern int pixelWidth;

	extern Renderer renderer;

	extern float deltaTime;

	/// <summary>
	/// 游戏基类，一个抽象类
	/// </summary>
	class NoaGameEngine {
	public:
		enum GameWindowMode
		{
			FullScreen = SDL_WINDOW_FULLSCREEN,
			WindowMode = SDL_WINDOW_FOREIGN
		};

	private:
		std::chrono::system_clock::time_point tp1 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime;
		std::chrono::system_clock::time_point tp2 = chrono::system_clock::now();

		SDL_GLContext glContext = nullptr;
		SDL_Window* window = nullptr;
		SDL_Renderer* mainRenderer = nullptr;
		SDL_Texture* texture = nullptr;
		SDL_PixelFormat* format;

		void* pixelBuffer = nullptr;

		//窗口
		int width;
		int height;
		string gameName;
		GameWindowMode gameWindowMode;

		//像素宽度和高度
		int surfaceWidth;
		int surfaceHeight;

		bool isRun = true;

	protected:
		//float deltaTime = 0;

	public:
		virtual void Start() = 0;
		virtual void Update() = 0;

	public:

		//渲染器

		/// <summary>
		/// 游戏构造函数，用于创建游戏基本组件
		/// </summary>
		/// <param name="width">窗口宽度</param>
		/// <param name="height">窗口高度</param>
		/// <param name="windowMode">窗口模式</param>
		/// <param name="gameName">游戏名称</param>
		NoaGameEngine(
			int width, int height,
			GameWindowMode windowMode,
			string gameName
		);


		~NoaGameEngine();

		void GLRenderTexture();

		void* PixelBuffer();
		float DeltaTime();
		int Run();

		void MainThread(
			const int rigidbodyCount,
			const int behaviourCount,
			const int animatorCount
			);
		
		int Quit();

	};

	void Debug(string msg);
}



#endif // !NOAENGINE_H
