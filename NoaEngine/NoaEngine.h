#ifndef NOAENGINE_H
#define NOAENGINE_H

#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <chrono>
using namespace std;

#include <stdio.h>
#include <Windows.h>

#include "NoaMath.h"
#include <SDL2/SDL.h>
#include <iostream>

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

// Color conversion macro
#define BGR(color)	( (((color) & 0xFF) << 16) | ((color) & 0xFF00FF00) | (((color) & 0xFF0000) >> 16) )

extern SDL_Window* window;
extern SDL_Surface* surface;

extern float deltaTime;
extern int screenWidth;
extern int screenHeight;

extern void (*Start)(void);
extern void (*Update)(void);

//注册游戏初始化函数和主循环函数
#define SET_GAME_START(x) void (*Start)(void) = x
#define SET_GAME_LOOP(x) void(*Update)(void) = x

extern void DrawPixel(int x, int y, Uint32 color);
extern void Game(int width, int height);

extern bool GetKeyHold(char key);
extern bool GetKeyDown(char key);

extern Uint32 GetSpriteColor(float x, float y, int w, int h, Uint32* sprite);

#endif // !NOAENGINE_H
