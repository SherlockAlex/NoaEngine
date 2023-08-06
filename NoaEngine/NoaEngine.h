/*
	//a game example:

	#include "NoaEngine.h"

	static void YourGameStart(void)
	{
		//write your game init
	}

	static void YourGameUpdate(void){
		//write your game loop
	}

	NoaGameEngine game(1920,1080,NoaGameEngin::WindowMode,"Your Game Name",YourGameStart,YourGameUpdate);

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
	char* gameName;
	GameWindowMode gameWindowMode;

	//���ؿ�Ⱥ͸߶�
	int surfaceWidth;
	int surfaceHeight;

	//��Ϸ���
	void (*Start)(void);
	void (*Update)(void);

	float deltaTime = 0;

public:

	//��Ⱦ��
	Renderer renderer;
	
	NoaGameEngine(
		int width, int height,
		GameWindowMode windowMode,
		char* gameName,
		void (*Start)(void),void (*Update)(void)
	);
	~NoaGameEngine();
	void* PixelBuffer();
	int PixelWidth();
	int PixelHeight();
	float DeltaTime();
	int Run();

	void Debug(string msg);
	void Debug(vector<string> msg);

	SDL_Renderer* GetMainRenderer() {
		return mainRenderer;
	}

	SDL_Texture* GetSurface() {
		return texture;
	}

};

extern NoaGameEngine game;



typedef struct Ray {
	//����������ײ������Ϣ
	float angle = 0.0f;
	float distance = 0.0f;
	bool isHitDoor = false;
	bool isHitWall = false;

	//������ͼ��Ϣ
	Vector simple;

}Ray;


class Player {
public:
	Vector position;
	float angle = 0.0f;
	float FOV = PI * 0.25f;
	float viewDepth = 30.0f;

	float speed = 5.0f;

public:
	Player()
	{
		position.x = 15.0f;
		position.y = 5.9f;
	}
};

#define FPS_FUNCTION
extern Ray RayCastHit(
	int pixelX,			//���ص������
	Player& player,		//��Ҷ�������
	LevelMap& map		//��ǰ�ؿ���ͼ����
);



#endif // !NOAENGINE_H
