#include "NoaEngine.h"
#include <ctime>
#include <thread>
#include <mutex>
#include "SDL2/SDL_thread.h"

namespace noa {
	extern vector <Behaviour*> behaviours;
	extern vector<GameObject*> gameObjects;
	extern vector<Animator*> animatorList;
	extern vector<Rigidbody*> rigidbodys;

	//mutex mtx; // 定义互斥锁对象

	int pixelHeight = 0;
	int pixelWidth = 0;

	float deltaTime = 0;

	Renderer renderer;

	void NoaGameEngine::MainThread()
	{
		while (isRun)
		{
			//mtx.lock();
			tp2 = chrono::system_clock::now();
			elapsedTime = tp2 - tp1;

			deltaTime = elapsedTime.count();
			//执行游戏主类的update

			string windowTitle = gameName + " FPS: " + to_string(1 / deltaTime);
			SDL_SetWindowTitle(window, windowTitle.c_str());

			while (SDL_PollEvent(&ioEvent))
			{

				inputSystem.Update();

				if (ioEvent.type == SDL_QUIT)
				{
					isRun = false;
					SDL_Quit();
					return;
				}
			}

			for (int i = 0; i < rigidbodys.size(); i++)
			{
				rigidbodys[i]->RigidbodyUpdate(deltaTime);
			}

			Update();

			for (int i = 0; i < behaviours.size(); i++)
			{
				behaviours[i]->Update();
			}

			for (int i = 0; i < animatorList.size(); i++)
			{
				animatorList[i]->Update(deltaTime);
			}

			SDL_UnlockTexture(texture);
			SDL_RenderCopy(mainRenderer, texture, nullptr, nullptr);
			SDL_RenderPresent(mainRenderer);

			tp1 = tp2;
			//mtx.unlock();
		}
	}

	NoaGameEngine::NoaGameEngine(
		int width, int height,
		GameWindowMode windowMode,
		string gameName
	)
	{

		//初始化游戏
		this->width = width;
		this->height = height;
		this->gameWindowMode = windowMode;
		this->gameName = gameName;

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
			width,
			height
		);
		if (texture == nullptr) {
			exit(0);
		}

		int pitch;
		format = SDL_AllocFormat(SDL_PIXELFORMAT_BGR888);
		SDL_LockTexture(texture, nullptr, &pixelBuffer, &pitch);

		surfaceWidth = width;
		surfaceHeight = height;

		renderer = Renderer(surfaceWidth, surfaceHeight, pixelBuffer);

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

	int SDL_ThreadFunction(void* data) {
		// 从传递的data中获取std::function对象，并调用它
		std::function<void()>* funcPtr = static_cast<std::function<void()>*>(data);
		(*funcPtr)(); // 调用成员方法
		return 0;
	}

	int NoaGameEngine::Run()
	{
		//运行游戏

		tp1 = chrono::system_clock::now();
		chrono::duration<float> elapsedTime;
		tp2 = chrono::system_clock::now();

		

		Start();

		for (int i = 0; i < behaviours.size(); i++)
		{
			behaviours[i]->Start();
		}

		MainThread();

		return 0;
	}




	void Debug(string msg)
	{
		//下面显示时间的部分的代码可能编译会出现异常，注意在编译的预处理请加上 _CRT_SECURE_NO_WARNINGS
		std::time_t current_time = std::time(nullptr);

		// 将当前时间转换为字符串格式
		char time_string[100];
		std::strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", std::localtime(&current_time));
		cout << "[INFO " << time_string << "]:" << msg << endl;
	}
}



