#include "NoaEngine.h"

#include "Windows.h"

SDL_Window* window = nullptr;
SDL_Surface* surface = nullptr;

float deltaTime = 0.0f;

int surfaceWidth = 0;
int surfaceHeight = 0;

void DrawPixel(int x, int y, Uint32 color)
{
	Uint32* pixelBuffer = (Uint32*)surface->pixels;
	pixelBuffer[y * surface->w + x] = BGR(color);
}


void Game(int width, int height)
{
	int init = SDL_Init(SDL_INIT_EVERYTHING);
	if (init != 0)
	{
		printf("[error]:Game init failed\n");
		return;
	}

	SDL_Window* window = SDL_CreateWindow(
		"DEMO",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_FULLSCREEN
		//SDL_WINDOW_FOREIGN
	);
	if (window == nullptr)
	{
		printf("[error]:Create window faild\n");
		return;
	}

	surface = SDL_GetWindowSurface(window);
	if (surface == nullptr)
	{
		printf("[error]:Create Surface faild\n");
		return;
	}

	surfaceWidth = surface->w;
	surfaceHeight = surface->h;

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

		//游戏画面刷新
		SDL_UnlockSurface(surface);
		SDL_UpdateWindowSurface(window);
		//SDL_UpdateTexture(texture);

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
	if (GetAsyncKeyState((unsigned short)key) & 0x8000) {
		return true;
	}
	return false;
}

bool GetKeyDown(char key)
{
	//检测按键按下瞬间

	if (GetAsyncKeyState((unsigned short)key) & 0x0001) {
		return true;
	}
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
