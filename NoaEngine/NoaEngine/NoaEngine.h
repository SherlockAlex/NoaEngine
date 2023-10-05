/*
* ��Ŀ�����⣺
* (1)SDL2
* (2)sdl2_mixer.nuget
* (3)freetype2
* (4)freetype
* (5)nupengl
* 
* 
* NoaEngine��Ϸ������������:
* Animator					������
* Audio						����ģ��
* Actor						�ű�ģ��
* GameObject				��Ϸ��Ʒ,����һ��Sprite�Ϳ��Կ��ٹ��������е���Ϸ��Ʒ��������������Զ����Ƴ���
* InputSystem				����ϵͳ
* NoaEngine					��Ϸ����
* NoaMath					���õ���ѧģ��
* Renderer					��Ⱦ�������ڻ��ƻ���
* Scene						������ͼģ��
* Sprite					����ģ��
* Camera                    ���������������Ļ��ʾ������
* Resource					��Դ�����������𱾵���Դ�ļ�������
* Rigidbody					���壬��Ϸ�����ṩ��������������棬ʵ�������ƶ���tileMap����ײ����Լ�����֮�����ײ���
* Scene						�����࣬���Է���ʵ�ֳ�����tileMap���л��Լ���Ϸ��Ʒ�Ŀ��ٲ���
* 
* ǰ�ԣ�
* �������Linux�û�������ϸ�Ķ�Makefile�ļ��������ʹ�������ٵ����ÿ�������
* �������Windows�û����뿼��ʹ��VS 2022���п�����ע�����ú�SDL2�Ŀ�������
* 
* ��ͼ��
* ͬʱ���֧�ּ򵥵���Ƭ��ͼϵͳ�������ʹ��tiled������Լ�����Ƭ��ͼ��Ȼ�󱣴浽csv�ļ��У�
* ͬʱNoaTool��ͼƬת��Ϊ������Զ�ȡ��tsd��Ƭ��ʽ��ע����ʱ��֧�ֶ�ͼ�����Ƭ��ͼ����ع������ڿ�����
* �����ʹ���������õ�TileMapCamera����ʵ�����tile map��Ϸ
* 
* ��ʾ���ԣ�
* ��Ϸ����֧����ʾ���嵽��Ļ�ϣ�����Ŀǰ����֧��Ӣ������Ͱ���������
* 
* �ڴ����
* ��Ϸ������ڴ����������£�����Ϸ��ģ����вŷ�
* ��NoaEngine����InputSystem��Platform��Renderer��SceneManager��PhysicsSystem�Ĵ�ģ��
* 
* Scene Manager��
* SceneManager:����Scene�����ɣ��л����Լ����У�ж��
* Scene:�������µ�ÿ��Actor��PhysicsSystem��������Ľ���������ÿ��Actor���ڴ��ͷ�����
* Actor:��������ActorComponent���ڴ��ͷ�����
* �ڴ��ͷŵ�˳������Actor�ͷŵ��µ�ActorComponent��Ȼ����Scene�ͷ�����ע���Actor��Ȼ����SceneManager�ͷ����е�Scene
* 
* �ڼ�Rigidbody��Ȼ��һ���������࣬���ǵ�λ��ActorComponentһ������Actor�����ڴ���ͷ�
* ��Actor��ִ��UpdateComponent()֮ǰ����ִ��Rigidbody.Update()����Rigidbody���͸�Scene
* ͬʱActor��ÿһ֡����Scene�ϴ��Լ�ӵ�е�Rigidbody�����Scene����Update����ȥִ��ActorComponent.Update()
* Ȼ��ִ��PhysicsSystem.Update(),��������������и��������ײ��⴦��
* ֮��Scene����ִ��Rigidbodys.LateUpdate()��Actor.Update()
* 
* ���濪������:
* ���ȴ���һ����Ϸ���࣬��Ϸ�����Ƕ�NoaEngine�ľ���ʵ�֣�ʵ�ֳ��󷽷�NoaEngine.Start()��NoaEngine.Update()
* ����׶������ʹ��Renderer����Ļ�ϻ���ͼ�Σ��������Ҫʹ�������ṩ�Ĺ��ߣ�����밴��һ�²���
* (a) ��д���Լ�����Ϸ����
* (b) ��д���Լ�����Ϸ��ɫ(Actor)
* (c) ��д�Լ��Ľ�ɫ���(ActorComponent)������ʹ�������ṩ�����(StateMachine��SpriteRenderer��Animation)
* (d) ��������ص���Ϸ��ɫ�ϣ��������Actor�����������
* (e) ѡ������Ҫ��Camera�����Լ���дһ��Camera
* (f) ����Ϸ����ʵ������ĳ������󵽶��ϣ���ʹ��sceneManager.LoadScene("Your scene")
* (g) ��������������Լ�����Ϸ
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

//��������

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
	* ||    ��Ϸ���࣬һ��������   ||
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
