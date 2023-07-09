#include "NoaEngine.h"

SDL_Window* window = nullptr;
SDL_Surface* surface = nullptr;

float deltaTime = 0.0f;
int screenWidth = 1920 / 2;
int screenHeight = 1080 / 2;

void DrawPixel(int x, int y, Uint32 color)
{
	Uint32* pixelBuffer = (Uint32*)surface->pixels;
	pixelBuffer[y * screenWidth + x] = BGR(color);
}

void Game(int width, int height)
{
	int init = SDL_Init(SDL_INIT_EVERYTHING);
	if (init != 0)
	{
		return;
	}

	SDL_Window* window = SDL_CreateWindow(
		"DEMO",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		1920 / 2,
		1080 / 2,
		SDL_WINDOW_FOREIGN
	);
	if (window == nullptr)
	{
		return;
	}

	surface = SDL_GetWindowSurface(window);
	if (surface == nullptr)
	{
		return;
	}

	auto tp1 = chrono::system_clock::now();
	auto tp2 = chrono::system_clock::now();

	SDL_Event gameEvent;
	Start();

	while (1)
	{
		tp2 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		deltaTime = elapsedTime.count();

		SDL_PollEvent(&gameEvent);

		Update();
		SDL_UnlockSurface(surface);
		SDL_UpdateWindowSurface(window);

		if (gameEvent.type==SDL_QUIT)
		{
			SDL_Quit();
		}

	}

	SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);

}

bool GetKeyHold(char key) {
	//监听按键是否按住
	if (GetAsyncKeyState((unsigned short)key) & 0x8000)
		return true;
	return false;
}

bool GetKeyDown(char key)
{
	//检测按键按下瞬间
	if (GetAsyncKeyState((unsigned short)key) & 0x0001)
		return true;
	return false;
}

Uint32 GetSpriteColor(float x, float y, int w, int h, Uint32* sprite)
{
	//从图片中获取像素
	int sx = x * (float)w;
	int sy = y * (float)h - 1;
	//printf("%d,%d\n", sx, sy);
	if (sx<0 || sx>w || sy<0 || sy>h)
	{
		return BLACK;
	}

	return sprite[sy * w + sx];
}
