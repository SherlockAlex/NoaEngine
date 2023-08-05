#include "NoaEngine.h"
#include "Animator.h"
#include "Sprite.h"

float deltaTime = 0.0f;

SDL_Event ioEvent;

void DrawPixel(int x, int y, Uint32 color)
{
	if (x<0||x>=game.PixelWidth()||y<0||y>=game.PixelHeight())
	{
		return;
	}
	Uint32* pixel = (Uint32*)game.PixelBuffer();
	pixel[y * game.PixelWidth() + x] = color;
}

void DrawLine(nVector vetex1,nVector vetex2,Uint32 color)
{
	//在屏幕上画线
}

void DrawImage(
	int posX,
	int posY,
	int imageW,
	int imageH,
	int scaleForSurface,
	bool isDrawAlpha,
	Uint32* imageRGB
)
{
	
	//计算放大
	int wannaW = game.PixelWidth()/ scaleForSurface;
	int wannaH = (int)(((float)imageH / (float)imageW) * wannaW);

	Sprite sprite = Sprite(imageW, imageH, 1, imageRGB);

	for (int width = 0; width < wannaW; width++)
	{
		for (int height = 0; height < wannaH; height++)
		{
			int x = posX - ((float)wannaW * 0.5f) + width;
			int y = posY - ((float)wannaH * 0.5f) + height;

			float fSimpleX = (float)(width) / (float)wannaW;
			float fSimpleY = (float)(height) / (float)wannaH;

			//Uint32 pixelColor = GetSpriteColor(fSimpleY, fSimpleX, imageH, inmageW, imageRGB);
			Uint32 pixelColor = sprite.GetColor(fSimpleY, fSimpleX);
			if (isDrawAlpha) 
			{
				if (pixelColor == BLACK)
				{
					continue;
				}
			}
			
			if (x<0||x>=game.PixelWidth()||y<0||y>=game.PixelHeight())
			{
				continue;
			}
			DrawPixel(x, y, pixelColor);
		}
	}
}

//void Game(int width, int height)
//{
//	int init = SDL_Init(SDL_INIT_EVERYTHING);
//	if (init != 0)
//	{
//		printf("[error]:Game init failed\n");
//		return;
//	}
//
//	game.window = SDL_CreateWindow(
//		"DEMO",
//		SDL_WINDOWPOS_CENTERED,
//		SDL_WINDOWPOS_CENTERED,
//		width,
//		height,
//		gameWindowMode
//		//SDL_WINDOW_FULLSCREEN
//		//SDL_WINDOW_FOREIGN
//	);
//	if (game.window == nullptr)
//	{
//		printf("[error]:Create window faild\n");
//		return;
//	}
//
//	game.renderer = SDL_CreateRenderer(game.window, -1, SDL_RENDERER_ACCELERATED);
//	if (game.renderer == nullptr)
//	{
//		return;
//	}
//
//	game.texture = SDL_CreateTexture(game.renderer,
//		SDL_PIXELFORMAT_BGR888,
//		SDL_TEXTUREACCESS_STREAMING, 
//		//surface->w,
//		//surface->h
//		width,
//		height
//	);
//	if (game.texture==nullptr) {
//		return;
//	}
//
//	int pitch;
//	game.format = SDL_AllocFormat(SDL_PIXELFORMAT_BGR888);
//	SDL_LockTexture(game.texture, NULL, &game.pixelBuffer, &pitch);
//
//	//surfaceWidth = width;
//	//surfaceHeight = height;
//
//	surfaceWidth = width;
//	surfaceHeight = height;
//
//	//处理音频设备初始化
//	if (Mix_OpenAudio(
//		MIX_DEFAULT_FREQUENCY,
//		MIX_DEFAULT_FORMAT,
//		MIX_CHANNELS,
//		4096
//		)==-1)
//	{
//		printf("[error]:初始化音频设备失败\n");
//		return;
//	}
//
//	auto tp1 = chrono::system_clock::now();
//	chrono::duration<float> elapsedTime;
//	auto tp2 = chrono::system_clock::now();
//
//	Start();
//
//	while (1)
//	{
//		tp2 = chrono::system_clock::now();
//		elapsedTime = tp2 - tp1;
//		tp1 = tp2;
//		deltaTime = elapsedTime.count();
//		
//		SDL_PollEvent(&ioEvent);
//
//		Update();
//
//		for (int i = 0;i<animatorList.size();i++) 
//		{
//			animatorList[i]->Update();
//		}
//
//
//		SDL_UnlockTexture(game.texture);
//		SDL_RenderCopy(game.renderer, game.texture, NULL, NULL);
//		SDL_RenderPresent(game.renderer);
//
//		if (ioEvent.type==SDL_QUIT)
//		{
//			break;
//		}
//
//	}
//
//	free(game.pixelBuffer);
//	SDL_DestroyRenderer(game.renderer);
//	SDL_DestroyTexture(game.texture);
//	SDL_DestroyWindow(game.window);
//	SDL_Quit();
//}

NoaGameEngine::NoaGameEngine(
	int width,int height,
	GameWindowMode windowMode,
	char * gameName,
	void(*Start)(void), void (*Update)(void))
{
	
	//初始化游戏
	this->width = width;
	this->height = height;
	this->gameWindowMode = windowMode;
	this->gameName = gameName;

	//绑定Start和Update
	this->Start = Start;
	this->Update = Update;

	int init = SDL_Init(SDL_INIT_EVERYTHING);
	if (init != 0)
	{
		printf("[error]:Game init failed\n");
		exit(0);
	}

	window = SDL_CreateWindow(
		gameName,
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
		exit(0);
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == nullptr)
	{
		exit(0);
	}

	texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_BGR888,
		SDL_TEXTUREACCESS_STREAMING,
		//surface->w,
		//surface->h
		width,
		height
	);
	if (texture == nullptr) {
		exit(0);
	}

	int pitch;
	format = SDL_AllocFormat(SDL_PIXELFORMAT_BGR888);
	SDL_LockTexture(texture, NULL, &pixelBuffer, &pitch);

	//surfaceWidth = width;
	//surfaceHeight = height;

	surfaceWidth = width;
	surfaceHeight = height;

	//处理音频设备初始化
	if (Mix_OpenAudio(
		MIX_DEFAULT_FREQUENCY,
		MIX_DEFAULT_FORMAT,
		MIX_CHANNELS,
		4096
	) == -1)
	{
		printf("[error]:初始化音频设备失败\n");
		exit(0);
	}

}

NoaGameEngine::~NoaGameEngine() {
	free(pixelBuffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyTexture(texture);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void* NoaGameEngine::PixelBuffer() {
	return this->pixelBuffer;
}

int NoaGameEngine::PixelWidth() {
	return surfaceWidth;
}

int NoaGameEngine::PixelHeight() {
	return surfaceHeight;
}

float NoaGameEngine::DeltaTime() {
	return deltaTime;
}

int NoaGameEngine::Run()
{
	//运行游戏

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

		for (int i = 0; i < animatorList.size(); i++)
		{
			animatorList[i]->Update();
		}


		SDL_UnlockTexture(game.texture);
		SDL_RenderCopy(game.renderer, game.texture, NULL, NULL);
		SDL_RenderPresent(game.renderer);

		if (ioEvent.type == SDL_QUIT)
		{
			break;
		}

	}

	return 0;
}


Uint32* LoadTexture(const char* filename) {
	ifstream infile(filename, ios::binary);
	if (!infile) {
		cout << "无法打开文件！" << endl;
		return nullptr;
	}

	// 读取data的大小
	uint32_t size = 0;
	infile.read(reinterpret_cast<char*>(&size), sizeof(size));

	// 分配动态内存空间
	uint32_t* data = new uint32_t[size];

	// 读取vector内容
	infile.read(reinterpret_cast<char*>(data), sizeof(uint32_t) * size);

	infile.close();

	return data;

}

//射线投射算法
Ray RayCastHit(
	int pixelX,			//像素点横坐标
	Player& player,		//玩家对象引用
	LevelMap& map		//当前关卡地图引用
)
{
	//射线投射算法
	Ray ray;
	ray.distance = 0.0f;
	ray.angle = player.angle -
		player.FOV * (0.5f - (float)(pixelX) / (float)(game.PixelWidth()));
	const float rayForwordStep = 0.03f;
	const float eyeX = sinf(ray.angle);
	const float eyeY = cosf(ray.angle);

	while (!ray.isHitDoor && !ray.isHitWall && ray.distance < player.viewDepth)
	{
		ray.distance += rayForwordStep;

		const float floatHitPointX = player.position.x + ray.distance * eyeX;
		const float floatHitPointY = player.position.y + ray.distance * eyeY;

		const int intHitPointX = (int)floatHitPointX;
		const int intHitPointY = (int)floatHitPointY;

		if (intHitPointX < 0 || intHitPointX >= map.w || intHitPointY < 0 || intHitPointY >= map.h)
		{
			ray.isHitWall = true;
			ray.distance = player.viewDepth;
			continue;
		}

		const char hitChar = map.level[intHitPointY * map.w + intHitPointX];
		if (hitChar == 0 || (hitChar == 127))
		{

			if (hitChar == 0)
			{
				ray.isHitWall = true;
			}
			else if (hitChar == 127)
			{
				ray.isHitDoor = true;
			}

			const float fBlockMidX = (float)intHitPointX + 0.5f;
			const float fBlockMidY = (float)intHitPointY + 0.5f;
			const float fTestAngle = atan2f((floatHitPointY - fBlockMidY), (floatHitPointX - fBlockMidX));

			if (fTestAngle >= -PI * 0.25f && fTestAngle < PI * 0.25f)
			{
				ray.simple.x = floatHitPointY - (float)intHitPointY;

			}
			if (fTestAngle >= PI * 0.25f && fTestAngle < PI * 0.75f)
			{
				ray.simple.x = floatHitPointX - (float)intHitPointX;
			}
			if (fTestAngle < -PI * 0.25f && fTestAngle >= -PI * 0.75f)
			{
				ray.simple.x = floatHitPointX - (float)intHitPointX;
			}
			if (fTestAngle >= PI * 0.75f || fTestAngle < -PI * 0.75f)
			{
				ray.simple.x = floatHitPointY - (float)intHitPointY;
			}
		}
	}

	ray.distance = ray.distance * cosf(player.angle - ray.angle);

	return ray;

}


