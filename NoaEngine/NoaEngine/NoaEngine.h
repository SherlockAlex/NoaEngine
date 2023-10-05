/*
* 项目依赖库：
* (1)SDL2
* (2)sdl2_mixer.nuget
* (3)freetype2
* (4)freetype
* (5)nupengl
* 
* 
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
* 前言：
* 如果你是Linux用户，请仔细阅读Makefile文件，你可以使用它快速的配置开发环境
* 如果你是Windows用户，请考虑使用VS 2022进行开发，注意配置好SDL2的开发环境
* 
* 地图：
* 同时软件支持简单的瓦片地图系统，你可以使用tiled设计你自己的瓦片地图，然后保存到csv文件中，
* 同时NoaTool将图片转换为引擎可以读取的tsd瓦片格式，注意暂时不支持多图层的瓦片地图，相关功能正在开发中
* 你可以使用引擎内置的TileMapCamera快熟实现你的tile map游戏
* 
* 显示语言：
* 游戏引擎支持显示字体到屏幕上，但是目前仅仅支持英文字体和阿拉伯数字
* 
* 内存管理：
* 游戏引擎的内存管理策略如下，将游戏的模块进行才分
* 由NoaEngine管理InputSystem，Platform，Renderer，SceneManager，PhysicsSystem四大模块
* 
* Scene Manager：
* SceneManager:管理Scene的生成，切换，以及运行，卸载
* Scene:管理场景下的每个Actor，PhysicsSystem物理世界的建立，负责每个Actor的内存释放问题
* Actor:负责所有ActorComponent的内存释放问题
* 内存释放的顺序，先是Actor释放底下的ActorComponent，然后由Scene释放所有注册的Actor，然后由SceneManager释放所有的Scene
* 
* 期间Rigidbody虽然是一个单独的类，但是地位和ActorComponent一样，由Actor进行内存的释放
* 由Actor在执行UpdateComponent()之前会先执行Rigidbody.Update()，将Rigidbody发送给Scene
* 同时Actor会每一帧的向Scene上传自己拥有的Rigidbody组件，Scene会在Update中先去执行ActorComponent.Update()
* 然后执行PhysicsSystem.Update(),对物理世界的所有刚体进行碰撞检测处理
* 之后Scene会先执行Rigidbodys.LateUpdate()和Actor.Update()
* 
* 引擎开发过程:
* 首先创建一个游戏主类，游戏主类是对NoaEngine的具体实现，实现抽象方法NoaEngine.Start()和NoaEngine.Update()
* 这个阶段你可以使用Renderer在屏幕上绘制图形，但是如果要使用引擎提供的工具，你必须按照一下步骤
* (a) 编写你自己的游戏场景
* (b) 编写你自己的游戏角色(Actor)
* (c) 编写自己的角色组件(ActorComponent)，或者使用引擎提供的组件(StateMachine、SpriteRenderer、Animation)
* (d) 将组件挂载到游戏角色上，即让你的Actor引用组件对象
* (e) 选择你想要的Camera或着自己编写一个Camera
* (f) 在游戏主类实例化你的场景对象到堆上，并使用sceneManager.LoadScene("Your scene")
* (g) 点击编译运行你自己的游戏
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
			WINDOWMODE windowMode,
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
		ExampleGame game(1920,1080,WINDOWMODE::WINDOW,"ExampleGame");
		return game.Run();
	}

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

#include "Platform.h"
#include "Transform.h"
#include "Debug.h"
#include "NoaMath.h"
#include "Scene.h"
#include "Audio.h"
#include "InputSystem.h"
#include "Actor.h"
#include "Animator.h"
#include "Sprite.h"
#include "Physics.h"
#include "NoaAction.h"
#include "Camera.h"
#include "StateMachine.h"
#include "NoaGUI.h"
#include "Resource.h"
#include "ScriptableActor.h"
#include "ActorComponent.h"
#include "NObject.h"
#include "Time.h"
#include "Texture.h"
#include "Renderer.h"
#include "SpriteRenderer.h"

#include "Collider2D.h"
#include "PhysicsSystem.h"

#ifdef _WIN64

#include "Platform_Windows.h"

#include "GLRenderer.h"
#include "GLTexture.h"

#endif

//窗口属性

namespace noa {

	extern int pixelHeight;
	extern int pixelWidth;

	extern std::shared_ptr<Renderer> renderer;

#pragma region OPENGL


	typedef struct SpriteGPUInstance
	{
		Texture* texture;
		Vector<int> position;
		Vector<int> scale;
		float eulerAngle = 0.0;
		bool flip = false;

	}SpriteGPUInstance;

	extern std::vector<SpriteGPUInstance> spriteInstances;

	/*
	* o--------------------------o
	* ||    游戏基类，一个抽象类   ||
	* o--------------------------o
	*/
	class NoaEngine
	{

	private:
		std::chrono::system_clock::time_point tp1 = std::chrono::system_clock::now();
		std::chrono::system_clock::time_point tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp1 - tp2;

		Texture* texture = nullptr;
		SDL_Window* window = nullptr;

	public:
		NoaEngine(
			int width, int height,
			WindowMode windowMode,
			std::string gameName
		);

		virtual ~NoaEngine();

		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual void OnDisable() {};

		int Run();

		void EngineThread();
		void EventLoop();

		int Quit();

	};

#pragma endregion

}



#endif // !NOAENGINE_H
