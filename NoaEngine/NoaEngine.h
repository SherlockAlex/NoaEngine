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

	SET_GAME_START(YourGameStart)
	SET_GAME_LOOP(YourGameUpdate)
	SET_GAME_WINDOW(windowWidth,windowHeight,GameWindowMode)

*/


#ifndef NOAENGINE_H
#define NOAENGINE_H

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
using namespace std;

#include <stdio.h>

//Windows System
#ifdef _WIN64
#include <Windows.h>
#endif

#include "NoaMath.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <iostream>
#include <list>

// Color constant
#define	BLACK			0
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

//取低位
#define LOBYTE(w)           ((Uint8)(w&0xff))

// Color conversion macro
#define BGR(color)	( (((color) & 0xFF) << 16) | ((color) & 0xFF00FF00) | (((color) & 0xFF0000) >> 16) )

#define RGB(r,g,b)  ((Uint32)(((Uint8)(r)|((Uint8)((Uint8)(g))<<8))|(((unsigned long)(Uint8)(b))<<16)))

#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((unsigned short)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))

extern float deltaTime;

enum GameWindowMode
{
	FullScreen = SDL_WINDOW_FULLSCREEN,
	WindowMode = SDL_WINDOW_FOREIGN
};
extern int windowWidth;
extern int windowHeight;
extern GameWindowMode gameWindowMode;

extern int surfaceWidth;
extern int surfaceHeight;

extern SDL_Event ioEvent;

extern void (*Start)(void);
extern void (*Update)(void);

//注册游戏初始化函数和主循环函数
#define SET_GAME_START(x) void (*Start)(void) = x;
#define SET_GAME_LOOP(x) void(*Update)(void) = x;

#define SET_GAME_WINDOW(w,h,flag) int windowWidth = w; int windowHeight = h;GameWindowMode gameWindowMode = flag;

extern void DrawPixel(int x, int y, Uint32 pixelColor);

extern void DrawImage(
	int posX,
	int posY,
	int inmageW,
	int imageH,
	int scaleForSurface,
	bool isDrawAlpha,
	Uint32* imageRGB);

extern void Game(int width, int height);

extern bool GetKeyHold(char key);
extern bool GetKeyDown(char key);

extern Uint32 GetSpriteColor(
	float normalizedX,
	float normalizedY, 
	int imageW, 
	int imageH,
	Uint32* sprite);

/// <summary>
/// 游戏物品结构体
/// </summary>
typedef struct GameObject {
	Vector positon;
	int width = 6;			
	int height = 6;
	Vector velocity;
	Uint32* imageRGB;
	bool isCollider = false;
}GameObject;

typedef struct LevelMap {
	char* currentMap;
	int w;
	int h;
}LevelMap;

//动画器
class Animator
{
private:
	vector<Uint32*> framesImage;
	Uint32* currentFrame;

public:
	Animator();
	Animator(Uint32* frameImage);
	~Animator();

public:
	Uint32* GetCurrentFrameImage();
	Uint32* GetFrameImage(int frame);
	void Play(int frame);
	void InsertFrameImage(Uint32* frameImage);
	Uint32 GetCurrentFramePixel(int index);
};

//按键映射
enum KEYCODE
{
	KeyA = 'A',
	KeyB = 'B',
	KeyC = 'C',
	KeyD = 'D',
	KeyE = 'E',
	KeyF = 'F',
	KeyG = 'G',
	KeyH = 'H',
	KeyI = 'I',
	KeyJ = 'J',
	KeyK = 'K',
	KeyL = 'L',
	KeyM = 'M',
	KeyN = 'N',
	KeyO = 'O',
	KeyP = 'P',
	KeyQ = 'Q',
	KeyR = 'R',
	KeyS = 'S',
	KeyT = 'T',
	KeyU = 'V',
	KeyW = 'W',
	KeyX = 'X',
	KeyY = 'Y',
	KeyZ = 'Z',
};

//精灵贴图
class Sprite {

private:
	int w = 1;
	int h = 1;
	int sizeForSurface = 3;
	Uint32* image = nullptr;

public:
	Sprite();
	Sprite(int w,int h,int size,Uint32 * image);
	~Sprite();

public:
	void UpdateImage(Uint32 * image);
	void DrawSprite(int posX,int posY,bool isRenderAlpha);

};

inline Sprite::Sprite() {

}

inline Sprite::Sprite(int w, int h,int size, Uint32* image) 
{
	this->w = w;
	this->h = h;
	this->image = image;
	this->sizeForSurface = size;
}

inline Sprite::~Sprite() {

}

inline void Sprite::UpdateImage(Uint32* image)
{
	this->image = image;
}

inline void Sprite::DrawSprite(int posX, int posY, bool isRenderAlpha)
{
	//计算放大
	int wannaW = surfaceWidth / sizeForSurface;
	int wannaH = (int)(((float)h / (float)w) * wannaW);

	for (int width = 0; width < wannaW; width++)
	{
		for (int height = 0; height < wannaH; height++)
		{
			int x = posX - ((float)wannaW * 0.5f) + width;
			int y = posY - ((float)wannaH * 0.5f) + height;

			float fSimpleX = (float)(width) / (float)wannaW;
			float fSimpleY = (float)(height) / (float)wannaH;

			Uint32 pixelColor = GetSpriteColor(fSimpleY, fSimpleX, h, w, image);

			if (isRenderAlpha)
			{
				if (pixelColor == BLACK)
				{
					continue;
				}
			}

			if (x < 0 || x >= surfaceWidth || y < 0 || y >= surfaceHeight)
			{
				continue;
			}
			DrawPixel(x, y, pixelColor);
		}
	}
}

#endif // !NOAENGINE_H
