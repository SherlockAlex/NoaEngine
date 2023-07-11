#include "NoaEngine.h"

SDL_Window* window = nullptr;
//SDL_Surface* surface = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* texture = nullptr;
SDL_PixelFormat* format;
void* pixelBuffer = nullptr;

float deltaTime = 0.0f;

int surfaceWidth = 0;
int surfaceHeight = 0;

SDL_Event ioEvent;

void DrawPixel(int x, int y, Uint32 color)
{
	if (x<0||x>=surfaceWidth||y<0||y>=surfaceHeight)
	{
		return;
	}
	Uint32* pixel = (Uint32*)pixelBuffer;
	pixel[y * surfaceWidth + x] = color;
}

void DrawImage(
	int posX,
	int posY,
	int inmageW,
	int imageH,
	int scaleForSurface,
	bool isDrawAlpha,
	Uint32* imageRGB
)
{
	
	//计算放大
	int wannaW = surfaceWidth/ scaleForSurface;
	int wannaH = (int)(((float)imageH / (float)inmageW) * wannaW);

	for (int width = 0; width < wannaW; width++)
	{
		for (int height = 0; height < wannaH; height++)
		{
			int x = posX - ((float)wannaW * 0.5f) + width;
			int y = posY - ((float)wannaH * 0.5f) + height;

			float fSimpleX = (float)(width) / (float)wannaW;
			float fSimpleY = (float)(height) / (float)wannaH;

			Uint32 pixelColor = GetSpriteColor(fSimpleY, fSimpleX, imageH, inmageW, imageRGB);

			if (isDrawAlpha) 
			{
				if (pixelColor == BLACK)
				{
					continue;
				}
			}
			
			if (x<0||x>=surfaceWidth||y<0||y>=surfaceHeight)
			{
				continue;
			}
			DrawPixel(x, y, pixelColor);
		}
	}
}

void Game(int width, int height)
{
	int init = SDL_Init(SDL_INIT_EVERYTHING);
	if (init != 0)
	{
		printf("[error]:Game init failed\n");
		return;
	}

	window = SDL_CreateWindow(
		"DEMO",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		gameWindowMode
		//SDL_WINDOW_FULLSCREEN
		//SDL_WINDOW_FOREIGN
	);
	if (window == nullptr)
	{
		printf("[error]:Create window faild\n");
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		return;
	}

	texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_BGR888,
		SDL_TEXTUREACCESS_STREAMING, 
		//surface->w,
		//surface->h
		width,
		height
	);
	if (texture==nullptr) {
		return;
	}

	int pitch;
	format = SDL_AllocFormat(SDL_PIXELFORMAT_BGR888);
	SDL_LockTexture(texture, NULL, &pixelBuffer, &pitch);

	//surfaceWidth = width;
	//surfaceHeight = height;

	surfaceWidth = width;
	surfaceHeight = height;

	

	auto tp1 = chrono::system_clock::now();
	chrono::duration<float> elapsedTime;
	auto tp2 = chrono::system_clock::now();

	Start();

	while (1)
	{
		tp2 = chrono::system_clock::now();
		elapsedTime = tp2 - tp1;
		tp1 = tp2;
		deltaTime = elapsedTime.count();
		
		SDL_PollEvent(&ioEvent);

		Update();

		//SDL_UnlockSurface(surface);
		//SDL_UpdateWindowSurface(window);
		SDL_UnlockTexture(texture);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);

		if (ioEvent.type==SDL_QUIT)
		{
			break;
		}

	}

	free(pixelBuffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(texture);
	//SDL_FreeSurface(surface);
	SDL_DestroyWindow(window);
	SDL_Quit();
}


bool GetKeyHold(char key) {
	//监听按键是否按住

#ifdef _WIN64
	if (GetAsyncKeyState((unsigned short)key) & 0x8000) {
		return true;
	}

#else 
#ifdef __linux__
	
	if (ioEvent.type != SDL_KEYDOWN)
	{
		return false;
	}
	if (ioEvent.key.keysym.sym == (key+32))
	{
		SDL_PollEvent(&ioEvent);
		printf("key %c is hold on\n",key);
		return true;
	}
#endif // LINUX

#endif
	return false;
}

bool GetKeyDown(char key)
{
	//检测按键按下瞬间
#ifdef _WIN64
	if (GetAsyncKeyState((unsigned short)key) & 0x0001) {
		return true;
	}
#else
#ifdef __linux__
	if (ioEvent.type != SDL_KEYDOWN)
	{
		return false;
	}
	if (ioEvent.key.keysym.sym == (key + 32))
	{
		printf("key %c is pressed\n", key);
		SDL_PollEvent(&ioEvent);
		return true;
	}
#endif // _LINUX

#endif // _WIN64
	return false;

}

Uint32 GetSpriteColor(float normalizedX, float normalizedY, int imageW, int imageH, Uint32* sprite)
{
	//从图片中获取像素
	int sx = normalizedX * (float)imageW;
	int sy = normalizedY * (float)imageH - 1;
	//printf("%d,%d\n", sx, sy);
	if (sx<0 || sx>imageW || sy<0 || sy>imageH)
	{
		return BLACK;
	}

	return sprite[sy * imageW + sx];
}

