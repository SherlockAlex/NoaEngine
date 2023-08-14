/*
* NoaEngine游戏引擎内容如下
* Animator
* Audio
* Behaviour
* GameObject
* InputSystem
* NoaEngine
* NoaMath
* Renderer
* Scene
* Sprite
* 
* 其中还有一个游戏例子d_*开头的项目
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
using namespace std;

#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_thread.h>
#include <list>
#include <unordered_map>

#include <fstream>
#include <string>

#include "NoaMath.h"
#include "Scene.h"
#include "Audio.h"
#include "InputSystem.h"
#include "Renderer.h"
#include "Behaviour.h"
#include "GameObject.h"

//窗口属性

extern int pixelHeight;
extern int pixelWidth;

extern Renderer renderer;

//游戏主类
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

	//窗口
	int width;
	int height;
	string gameName;
	GameWindowMode gameWindowMode;

	//像素宽度和高度
	int surfaceWidth;
	int surfaceHeight;

	float deltaTime = 0;

public:
	virtual void Start() = 0;
	virtual void Update() = 0;

public:

	//渲染器
	
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


//extern NoaGameEngine game;

#endif // !NOAENGINE_H
