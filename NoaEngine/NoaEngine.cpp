#include "NoaEngine.h"
#include "Animator.h"
#include "Sprite.h"

//float deltaTime = 0.0f;

int pixelHeight = 0;
int pixelWidth = 0;

Renderer renderer;

NoaGameEngine::NoaGameEngine(
	int width,int height,
	GameWindowMode windowMode,
	string gameName
	)
{
	
	//初始化游戏
	this->width = width;
	this->height = height;
	this->gameWindowMode = windowMode;
	this->gameName = gameName;

	//绑定Start和Update
	//this->Start = Start;
	//this->Update = Update;

	int init = SDL_Init(SDL_INIT_EVERYTHING);
	if (init != 0)
	{
		Debug("Game init failed");
		exit(0);
	}

	window = SDL_CreateWindow(
		gameName.c_str(),
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
		Debug("Create window faild");
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
		Debug("初始化音频设备失败");
		exit(0);
	}

	pixelWidth = surfaceWidth;
	pixelHeight = surfaceHeight;

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

float NoaGameEngine::DeltaTime() {
	return deltaTime;
}

//刷新游戏画面

//计算每一帧需要处理几个I0事件
float EventStep(const float deltaTime) 
{
	return 1000 * deltaTime;
}

int InputThread(NoaGameEngine * game) 
{
	//主要的线程
	const float deltaTime = game->DeltaTime();
	const float step = EventStep(deltaTime);
	int count = 0;
	for (float i = 0; i < step *deltaTime;i+=deltaTime)
	{
		SDL_PollEvent(&ioEvent);
		count++;
	}

	//cout <<"fps:"<<1/deltaTime << "deltaTime:" << deltaTime << ",count" << count << endl;
	
	return 0;
}

int NoaGameEngine::Run()
{
	//运行游戏

	auto tp1 = chrono::system_clock::now();
	chrono::duration<float> elapsedTime;
	auto tp2 = chrono::system_clock::now();

	// 创建独立线程，处理输入事件

	Start();

	for (int i = 0; i < behaviours.size(); i++)
	{
		behaviours[i]->Start();
	}

	while (1)
	{
		tp2 = chrono::system_clock::now();
		elapsedTime = tp2 - tp1;
		tp1 = tp2;
		deltaTime = elapsedTime.count();

		InputThread(this);
		
		//执行游戏主类的update
		Update();

		for (int i = 0; i < behaviours.size(); i++)
		{
			behaviours[i]->Update();
		}

		for (int i = 0; i < animatorList.size(); i++)
		{
			animatorList[i]->Update(this->deltaTime);
		}

		if (ioEvent.type == SDL_QUIT)
		{
			break;
		}

		SDL_UnlockTexture(texture);
		SDL_RenderCopy(mainRenderer, texture, nullptr, nullptr);
		SDL_RenderPresent(mainRenderer);

	}

	return 0;
}

void Debug(string msg)
{
	cout << "[INFO]:" << msg << endl;
}

void Debug(vector<string> msg)
{
	cout << "[INFO]:";
	for (int i = 0;i<msg.size();i++)
	{
		cout << msg[i];
	}
	cout << endl;
}



