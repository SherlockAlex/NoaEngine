/*
* NoaEngine��Ϸ������������:
* Animator					������
* Audio						����ģ��
* Behaviour					�ű�ģ��
* GameObject				��Ϸ��Ʒ
* InputSystem				����ϵͳ
* NoaEngine					��Ϸ����
* NoaMath					���õ���ѧģ��
* Renderer					��Ⱦ�������ڻ��ƻ���
* Scene						������ͼģ��
* Sprite					����ģ��
* 
* ���л���һ����Ϸ����d_*��ͷ����Ŀ
* 
*/

/*
	//a game example:

	#include "NoaEngine.h"

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

//��������
extern int pixelHeight;
extern int pixelWidth;

extern Renderer renderer;

extern float deltaTime;

//��Ϸ����
class NoaGameEngine {
public:
	enum GameWindowMode
	{
		FullScreen = SDL_WINDOW_FULLSCREEN,
		WindowMode = SDL_WINDOW_FOREIGN
	};

private:
	SDL_Window* window = nullptr;
	SDL_Renderer* mainRenderer = nullptr;
	SDL_Texture* texture = nullptr;
	SDL_PixelFormat* format;
	void* pixelBuffer = nullptr;

	//����
	int width;
	int height;
	string gameName;
	GameWindowMode gameWindowMode;

	//���ؿ�Ⱥ͸߶�
	int surfaceWidth;
	int surfaceHeight;

protected:
	//float deltaTime = 0;

public:
	virtual void Start() = 0;
	virtual void Update() = 0;

public:

	//��Ⱦ��
	
	NoaGameEngine(
		int width, int height,
		GameWindowMode windowMode,
		string gameName
	);
	~NoaGameEngine();
	void* PixelBuffer();
	float DeltaTime();
	int Run();

};

void Debug(string msg);

void Debug(vector<string> msg);

#endif // !NOAENGINE_H
