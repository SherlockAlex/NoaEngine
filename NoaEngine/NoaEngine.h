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

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_gamecontroller.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <list>
#include <unordered_map>

#include <fstream>
#include <string>

// Math Constance
#define PI 3.14159

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

//取低位
#define LOBYTE(w)           ((Uint8)(w&0xff))

// Color conversion macro
#define BGR(color)	( (((color) & 0xFF) << 16) | ((color) & 0xFF00FF00) | (((color) & 0xFF0000) >> 16) )

#define RGB(r,g,b)  ((Uint32)(((Uint8)(r)|((Uint8)((Uint8)(g))<<8))|(((unsigned long)(Uint8)(b))<<16)))

#define GetRValue(rgb)      (LOBYTE(rgb))
#define GetGValue(rgb)      (LOBYTE(((unsigned short)(rgb)) >> 8))
#define GetBValue(rgb)      (LOBYTE((rgb)>>16))

extern float deltaTime;

typedef struct GameWindow {
	//图形相关
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* texture = nullptr;
	SDL_PixelFormat* format;
	void* pixelBuffer = nullptr;

	//声音相关
}GameWindow;

typedef void(*eventFunc)(void);

//定义2维矢量
typedef struct Vector {
	float x = 0.0f;
	float y = 0.0f;
}Vector;

typedef struct nVector {
	int x = 0;
	int y = 0;
}nVector;


/// <summary>
/// 游戏物品结构体
/// </summary>
typedef struct GameObject {
	Vector positon;
	int width = 6;			//宽度
	int height = 6;			//高度
	Vector velocity;		//速度
	Uint32* imageRGB;		//颜色
	bool isCollider = false;
}GameObject;

typedef struct Ray {
	//返回射线碰撞到的信息
	float angle = 0.0f;
	float distance = 0.0f;
	bool isHitDoor = false;
	bool isHitWall = false;

	//返回贴图信息
	Vector simple;

}Ray;


class LevelMap {
private:
	
	

public:
	char* level = nullptr;
	int w;
	int h;
public:
	LevelMap() {

	}

	LevelMap(int width, int height, wstring map)
	{
		this->w = width;
		this->h = height;

		level = (char*)malloc(sizeof(char) * width*height);

		//加载地图到缓存中
		for (int i = 0; i < map.length(); i++)
		{
			level[i] = map[i];
		}

	}

	void LoadMap(wstring map)
	{
		for (int i = 0; i < map.length(); i++)
		{
			level[i] = map[i];
		}
	}

	std::wstring LoadLevel(const char* filePath) {
		std::wstring content;

		std::ifstream file(filePath, std::ios::binary);

		if (file.is_open()) {
			file.seekg(0, std::ios::end);
			size_t length = file.tellg();
			file.seekg(0, std::ios::beg);

			char* buffer = new char[length];
			file.read(buffer, length);

			content = std::wstring(reinterpret_cast<const wchar_t*>(buffer), length / sizeof(wchar_t));

			delete[] buffer;

			file.close();
		}
		else {
			std::cerr << "无法打开文件。" << std::endl;
		}

		return content;
	}

	void SaveLevel(const std::wstring& map,const char * filePath) {
		std::ofstream file(filePath, std::ios::binary);

		if (file.is_open()) {
			const char* bytes = reinterpret_cast<const char*>(map.c_str());
			size_t length = sizeof(wchar_t) * map.length();

			file.write(bytes, length);
			file.close();

			std::cout << "内容已成功保存到二进制文件中。" << std::endl;
		}
		else {
			std::cerr << "无法打开文件。" << std::endl;
		}
	}

};



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
extern Uint32 GetSpriteColor(
	float normalizedX,
	float normalizedY,
	int imageW,
	int imageH,
	Uint32* sprite);

/// <summary>
/// 从本地二进制文件spr中加载贴图数据
/// </summary>
/// <param name="filename">二进制文件路径</param>
/// <returns></returns>
extern Uint32* LoadTexture(const char * filename);

#define GameEngine

enum GameWindowMode
{
	FullScreen = SDL_WINDOW_FULLSCREEN,
	WindowMode = SDL_WINDOW_FOREIGN
};
extern int windowWidth;
extern int windowHeight;
extern GameWindowMode gameWindowMode;
extern void (*Start)(void);
extern void (*Update)(void);

//注册游戏初始化函数和主循环函数
#define SET_GAME_START(x) void (*Start)(void) = x;
#define SET_GAME_LOOP(x) void(*Update)(void) = x;
#define SET_GAME_WINDOW(w,h,flag) int windowWidth = w; int windowHeight = h;GameWindowMode gameWindowMode = flag;
extern void Game(int width, int height);

#define INPUT
extern bool GetKeyHold(char key);
extern bool GetKeyDown(char key);

//动画器
class Animator
{
private:
	vector<Uint32*> framesImage;
	unordered_map<Uint32, eventFunc> framesEvent;
	Uint32* currentFrame;
	bool isPlaying = false;
	//动画播放速度
	float speed = 7;
	float i = 0;

public:
	Animator(float speed);
	Animator(Uint32* frameImage,float speed);
	~Animator();

public:
	void LoadFromAnimatorFile(const char * filePath);
	Uint32* GetCurrentFrameImage();
	Uint32* GetFrameImage(int frame);
	void SetFrameEvent(int frame,eventFunc e);
	void Play(int frame);
	void Play();
	void InsertFrameImage(Uint32* frameImage);
	Uint32 GetCurrentFramePixel(int index);
	void Update();//更新动画帧

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

enum AudioType {
	Music,
	Chunk
};

class Audio {
private:
	AudioType type = Music;
	Mix_Music* music = nullptr;
	Mix_Chunk* chunk = nullptr;
public:
	Audio(const char * filePath,AudioType type);
	~Audio();
public:
	void Play(bool loop);
};

#define FPS_FUNCTION
extern Ray RayCastHit(
	int pixelX,			//像素点横坐标
	Player& player,		//玩家对象引用
	LevelMap& map		//当前关卡地图引用
);


#endif // !NOAENGINE_H
