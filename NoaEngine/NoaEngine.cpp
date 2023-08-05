#include "NoaEngine.h"
#include "Animator.h"
#include "Sprite.h"

float deltaTime = 0.0f;

SDL_Event ioEvent;

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

	mainRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (mainRenderer == nullptr)
	{
		exit(0);
	}

	texture = SDL_CreateTexture(mainRenderer,
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

	surfaceWidth = width;
	surfaceHeight = height;

	renderer = Renderer(surfaceWidth,surfaceHeight,pixelBuffer);

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
	SDL_DestroyRenderer(mainRenderer);
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

		//执行游戏主类的update
		Update();

		//执行所有Animator的Update
		for (int i = 0; i < animatorList.size(); i++)
		{
			animatorList[i]->Update();
		}


		SDL_UnlockTexture(game.texture);
		SDL_RenderCopy(game.mainRenderer, game.texture, NULL, NULL);
		SDL_RenderPresent(game.mainRenderer);

		if (ioEvent.type == SDL_QUIT)
		{
			break;
		}

	}

	return 0;
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


