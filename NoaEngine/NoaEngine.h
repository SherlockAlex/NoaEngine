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
#include <list>
#include <unordered_map>

#include <fstream>
#include <string>

#include "NoaMath.h"
#include "Scene.h"
#include "Audio.h"
#include "InputSystem.h"

// Color constant
#define	BLACK			0x000000
#define	BLUE			0xAA0000
#define	GREEN			0x00AA00
#define	CYAN			0xAAAA00
#define	RED				0x0000AA
#define	MAGENTA			0xAA00AA
#define	BROWN			0x0055AA
#define	LIGHTGRAY		0xAAAAAA
#define	DARKGRAY		0x555555
#define	LIGHTBLUE		0xFF5555
#define	LIGHTGREEN		0x55FF55
#define	LIGHTCYAN		0xFFFF55
#define	LIGHTRED		0x5555FF
#define	LIGHTMAGENTA	0xFF55FF
#define	YELLOW			0x55FFFF
#define	WHITE			0xFFFFFF

//ȡ��λ
#define LOBYTE(w)           ((Uint8)(w&0xff))

// Color conversion macro
#define BGR(color)	( (((color) & 0xFF) << 16) | ((color) & 0xFF00FF00) | (((color) & 0xFF0000) >> 16) )

#define RGB(r,g,b)  ((Uint32)(((Uint8)(r)|((Uint8)((Uint8)(g))<<8))|(((unsigned long)(Uint8)(b))<<16)))

#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((unsigned short)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))

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
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* texture = nullptr;
	SDL_PixelFormat* format;
	void* pixelBuffer = nullptr;

	//����
	int width;
	int height;
	char* gameName;
	GameWindowMode gameWindowMode;

	//��Ϸ���
	void (*Start)(void);
	void (*Update)(void);

public:

	float deltaTime;
	NoaGameEngine(
		int width, int height,
		GameWindowMode windowMode,
		char* gameName,
		void (*Start)(void),void (*Update)(void)
	);
	~NoaGameEngine();
	void* PixelBuffer();
	int Run();

};

extern NoaGameEngine game;

/// <summary>
/// ��Ϸ��Ʒ�ṹ��
/// </summary>
typedef struct GameObject {
	Vector positon;
	int width = 6;			//���
	int height = 6;			//�߶�
	Vector velocity;		//�ٶ�
	Uint32* imageRGB;		//��ɫ
	bool isCollider = false;
}GameObject;

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

	float speed = 8.0f;

public:
	Player()
	{
		position.x = 15.0f;
		position.y = 5.9f;
	}
};



extern int surfaceWidth;
extern int surfaceHeight;

extern SDL_Event ioEvent;

#define Graphic
extern void DrawPixel(int x, int y, Uint32 pixelColor);
extern void DrawImage(
	int posX,
	int posY,
	int inmageW,
	int imageH,
	int scaleForSurface,
	bool isDrawAlpha,
	Uint32* imageRGB);

/// <summary>
/// �ӱ��ض������ļ�spr�м�����ͼ����
/// </summary>
/// <param name="filename">�������ļ�·��</param>
/// <returns></returns>
extern Uint32* LoadTexture(const char * filename);

#define GameEngine


//extern int windowWidth;
//extern int windowHeight;
//extern GameWindowMode gameWindowMode;
//extern void (*Start)(void);
//extern void (*Update)(void);
//
////ע����Ϸ��ʼ����������ѭ������
//#define SET_GAME_START(x) void (*Start)(void) = x;
//#define SET_GAME_LOOP(x) void(*Update)(void) = x;
//#define SET_GAME_WINDOW(w,h,flag) int windowWidth = w; int windowHeight = h;GameWindowMode gameWindowMode = flag;
//extern void Game(int width, int height);


#define FPS_FUNCTION
extern Ray RayCastHit(
	int pixelX,			//���ص������
	Player& player,		//��Ҷ�������
	LevelMap& map		//��ǰ�ؿ���ͼ����
);


#endif // !NOAENGINE_H
