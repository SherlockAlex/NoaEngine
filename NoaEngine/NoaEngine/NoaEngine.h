/*
* 项目依赖库：
* (1)SDL2
* (2)sdl2_mixer.nuget
* (3)freetype2
* (4)freetype
* (5)nupengl
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

	class ExampleGame :public NoeEngine {
	public:
		ExampleGame(int width, int height,
			WindowMode windowMode,
			string gameName) :NoeEngine(width,height,windowMode,gameName) 
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
		ExampleGame game(1920,1080,WindowMode::WINDOW,"ExampleGame");
		return game.Run();
	}

*/

/**
* 阶段性任务：
* (a)完善UI系统和UI框架解决方案
* (b)优化物理引擎
* (c)跨平台配置部署
*/


#ifndef NOAENGINE_H
#define NOAENGINE_H

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

#include "SDLHelper.h"

#include "Screen.h"
#include "Platform.h"
#include "Transform.h"
#include "Debug.h"
#include "NoaMath.h"

#include "Scene.h"
#include "SceneManager.h"

#include "AudioSource.h"

#include "InputSystem.h"
#include "Actor.h"
#include "Animator.h"
#include "Sprite.h"
#include "Physics.h"
#include "NoaAction.h"

#include "Camera.h"
#include "Camera2D.h"

#include "State.h"
#include "StateMachine.h"

#include "NoaGUI.h"

#include "Resource.h"

#include "ActorComponent.h"
#include "NObject.h"
#include "Time.h"
#include "Texture.h"
#include "Renderer.h"

#include "SpriteRenderer.h"

#include "TileMapInfo.h"
#include "TileMapRenderer.h"
#include "TileMap.h"

#include "Collider2D.h"
#include "BoxCollider2D.h"
#include "CircleCollider2D.h"
#include "TileCollider2D.h"

#include "PhysicsSystem.h"
#include "ActorMotion.h"

#include "ObjectPool.h"
#include "IPoolObjectFactroy.h"

#include "ActorPool.h"
#include "ActorPoolFactory.h"


//窗口属性

namespace noa {

#pragma region OPENGL

	/*
	o--------------------------o
	||    游戏基类，一个抽象类   ||
	o--------------------------o
	*/
	enum class GraphicsAPI {
		SDL2,
		OPENGL
	};

	class NoaEngine
	{
	public:
		NoaEngine(
			int width, int height
			, WindowMode windowMode
			, GraphicsAPI graphics
			, const std::string& gameName
		);

		virtual ~NoaEngine();

		virtual void Start() {};
		virtual void Update() {};
		virtual void OnExit() {};

		int Run();
		

	private:
		void EngineThread();
		int Exit();
	private:
		std::chrono::system_clock::time_point tp1 = std::chrono::system_clock::now();
		std::chrono::system_clock::time_point tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp1 - tp2;

		Texture* texture = nullptr;
		uint32_t windowID = 0;
	};

	class Application {
	public:
		static void Quit();
	};

#pragma endregion

}



#endif // !NOAENGINE_H
