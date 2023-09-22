/*
* NoaEngine游戏引擎内容如下:
* Animator					动画器
* Audio						声音模块
* Actor						脚本模块
* GameObject				游戏物品,基于一个Sprite就可以快速构建场景中的游戏物品，并且相机可以自动绘制出来
* InputSystem				输入系统
* NoaEngine					游戏基类
* NoaMath					常用的数学模块
* Renderer					渲染器，用于绘制画面
* Scene						场景地图模块
* Sprite					精灵模块
* Camera                    摄像机，帮你解决屏幕显示的问题
* Resource					资源管理器，负责本地资源的加载问题
* Rigidbody					刚体，游戏引擎提供的物理简单物理引擎，实现人物移动，tileMap的碰撞检测以及刚体之间的碰撞检测
* Scene						场景类，可以方便实现场景和tileMap的切换以及游戏物品的快速布局
* 
* 如果你是Linux用户，请仔细阅读Makefile文件，你可以使用它快速的配置开发环境
* 如果你是Windows用户，请考虑使用VS 2022进行开发，注意配置好SDL2的开发环境
* 
* 同时软件支持简单的瓦片地图系统，你可以使用tiled设计你自己的瓦片地图，然后保存到csv文件中，
* 同时NoaTool将图片转换为引擎可以读取的tsd瓦片格式，注意暂时不支持多图层的瓦片地图，相关功能正在开发中
* 你可以使用引擎内置的TileMapCamera快熟实现你的tile map游戏
* 
* 游戏引擎支持显示字体到屏幕上，但是目前仅仅支持英文字体和阿拉伯数字
* 
*/

/*
	o-----------------------------------------o
	|										  |
	|				 A example   			  |
	|										  |
	o-----------------------------------------o

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
#include <queue>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_thread.h>

#include "include/glad/glad.h"

#include "Transform.h"
#include "NoaMath.h"
#include "Scene.h"
#include "Audio.h"
#include "InputSystem.h"
#include "Renderer.h"
#include "Actor.h"
#include "GameObject.h"
#include "Animator.h"
#include "Sprite.h"
#include "Physics.h"
#include "NoaAction.h"
#include "Camera.h"
#include "StateMachine.h"
#include "NoaGUI.h"
#include "Resource.h"
#include "ScriptableActor.h"

//窗口属性

namespace noa {
	extern int pixelHeight;
	extern int pixelWidth;

	extern Renderer renderer;

	extern float deltaTime;

	extern float gameTime;

	/*
	* o-----------------------——-o
	* |    游戏基类，一个抽象类     |
	* o------------------------——o
	*/
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
		SDL_Surface* surface = nullptr;
		SDL_Renderer* mainRenderer = nullptr;
		SDL_Texture* texture = nullptr;
		SDL_PixelFormat* format;

		//GLuint texture;

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
		virtual void OnDisable() {};
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

		// 更新OpenGL纹理像素数据并渲染
		void UpdateOpenGLTexture();
		void RenderOpenGLTexture();

		int Quit();

	};

	//线程池
	class ThreadPool {
	public:
		ThreadPool(size_t numThreads);
		~ThreadPool();

		// 添加任务到线程池
		void Enqueue(std::function<void()> task);

	private:
		// 线程函数
		void WorkerThread();

		// 线程池的线程数量
		size_t numThreads;

		// 存储线程对象的容器
		std::vector<std::thread> threads;

		// 任务队列
		std::queue<std::function<void()>> tasks;

		// 用于同步的互斥锁和条件变量
		std::mutex queueMutex;
		std::condition_variable condition;

		// 用于标志线程池是否关闭
		bool stop;
	};

	extern void Debug(string msg);
}



#endif // !NOAENGINE_H
