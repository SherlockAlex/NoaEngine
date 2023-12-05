/*
* ��Ŀ�����⣺
* (1)SDL2
* (2)sdl2_mixer.nuget
* (3)freetype2
* (4)freetype
* (5)nupengl
*/

/**
* �׶�������
* (a) ����UIϵͳ��UI��ܽ������
* (b) ���Ʋ��Ż���������
* (c) ��ƽ̨���ò���
* (d) �ṩ�����������չ�Ľӿ�(��ô�Ϳ���ӵ�б��ص����������box2d��������)
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

//��������

namespace noa {

#pragma region OPENGL

	/*
	o--------------------------o
	||    ��Ϸ���࣬һ��������   ||
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

		std::vector<EnginExtension*> extensions;//��Ϸ������չ

	};

#pragma endregion

}



#endif // !NOAENGINE_H
