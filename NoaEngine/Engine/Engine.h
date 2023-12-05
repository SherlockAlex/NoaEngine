/*
* 项目依赖库：
* (1)SDL2
* (2)sdl2_mixer.nuget
* (3)freetype2
* (4)freetype
* (5)nupengl
*/

/**
* 阶段性任务：
* (a) 完善UI系统和UI框架解决方案
* (b) 完善并优化物理引擎
* (c) 跨平台配置部署
* (d) 提供物理引擎可扩展的接口(那么就可以拥有本地的物理引擎和box2d物理引擎)
*/


#ifndef NOAENGINE_ENGINE_H
#define NOAENGINE_ENGINE_H

#include <iostream>
#include <chrono>
#include <unordered_map>
#include <string>
#include <vector>

#include "Application.h"
#include "Screen.h"

#include "Platform.h"
#include "Texture.h"
#include "Renderer.h"

#include "Transform.h"
#include "Debug.h"
#include "NoaMath.h"

#include "Scene.h"
#include "SceneManager.h"

#include "AudioSource.h"

#include "Input.h"

#include "NObject.h"
#include "Actor.h"
#include "ActorComponent.h"
#include "Animation.h"
#include "SpriteRenderer.h"

#include "Sprite.h"
#include "NoaAction.h"

#include "Camera.h"
#include "Camera2D.h"

#include "State.h"
#include "StateMachine.h"

#include "NoaGUI.h"

#include "Resource.h"

#include "Time.h"

#include "TileMapInfo.h"
#include "TileMapRenderer.h"
#include "TileMap.h"

#include "Collider2D.h"
#include "BoxCollider2D.h"
#include "CircleCollider2D.h"
#include "TileCollider2D.h"

#include "Rigidbody.h"
#include "PhysicsSystem.h"
#include "ActorMotion.h"

#include "ObjectPool.h"
#include "IPoolObjectFactroy.h"

#include "ActorPool.h"
#include "ActorPoolFactory.h"

#include "Core.h"

//窗口属性

namespace noa {

#pragma region OPENGL

	/*
	o--------------------------o
	||    游戏基类，一个抽象类   ||
	o--------------------------o
	*/
	enum class NOA_API GraphicsAPI {
		SDL2,
		OPENGL
	};

	class NOA_API EnginExtension;

	class NOA_API NoaEngine
	{
	public:
		NoaEngine(
			int width, int height
			, WindowMode windowMode
			, GraphicsAPI graphics
			, const std::string& gameName
		);
		virtual ~NoaEngine();

		void AddExtension(noa::EnginExtension* extension);
		void RemoveExtension(noa::EnginExtension* extension);

		int Run();

	protected:
		virtual void Start() {};
		virtual void BeforeUpdate() {};
		virtual void Update() {};
		virtual void Render() {};
		virtual void OnExit() {};

	private:

		void ExtensionStart();
		void ExtensionBeforeUpdate();
		void ExtensionUpdate();
		void ExtensionRender();
		void ExtensionOnExit();

		void EngineThread();
		int Exit();
	private:
		std::chrono::system_clock::time_point tp1 = std::chrono::system_clock::now();
		std::chrono::system_clock::time_point tp2 = std::chrono::system_clock::now();
		std::chrono::duration<float> elapsedTime = tp1 - tp2;

		Texture* texture = nullptr;
		uint32_t windowID = 0;

		std::vector<EnginExtension*> extensions;//游戏引擎扩展

	};

#pragma endregion

}



#endif // !NOAENGINE_H
