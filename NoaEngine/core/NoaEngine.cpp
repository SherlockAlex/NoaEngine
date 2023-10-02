#include "NoaEngine.h"
#include <ctime>
#include <thread>
#include <mutex>
#include <queue>

using namespace std;

namespace noa {

#ifdef _WIN64
	static shared_ptr<Platform> platform = make_shared<Platform_Windows>();
	shared_ptr<Renderer> renderer = make_shared<GLRenderer>();
	
#endif // _WIN64
	std::vector<SpriteGPUInstance> spriteInstances;

	int pixelHeight = 0;
	int pixelWidth = 0;

	uint32_t* pixelBuffer = nullptr;

	
	int pitch;

#pragma region SDL

	//NoaEngineSDL::NoaEngineSDL(
	//	int width, int height,
	//	WINDOWMODE windowMode,
	//	string gameName
	//)
	//{

	//	//初始化游戏
	//	this->width = width;
	//	this->height = height;
	//	this->gameWindowMode = windowMode;
	//	this->gameName = gameName;

	//	int init = SDL_Init(SDL_INIT_EVERYTHING);
	//	if (init != 0)
	//	{
	//		Debug("Game init failed");
	//		exit(-1);
	//	}

	//	window = SDL_CreateWindow(
	//		gameName.c_str(),
	//		SDL_WINDOWPOS_CENTERED,
	//		SDL_WINDOWPOS_CENTERED,
	//		width,
	//		height,
	//		(uint32_t)gameWindowMode
	//	);
	//	if (window == nullptr)
	//	{
	//		Debug("Create window faild");
	//		exit(-1);
	//	}

	//	surfaceWidth = width;
	//	surfaceHeight = height;

	//	pixelWidth = surfaceWidth;
	//	pixelHeight = surfaceHeight;

	//	mainRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	//	if (mainRenderer == nullptr)
	//	{
	//		exit(-1);
	//	}

	//	texture = SDL_CreateTexture(mainRenderer,
	//		SDL_PIXELFORMAT_ABGR8888,
	//		SDL_TEXTUREACCESS_STREAMING,
	//		width,
	//		height
	//	);
	//	if (texture == nullptr) {
	//		exit(-1);
	//	}

	//	format = SDL_AllocFormat(SDL_PIXELFORMAT_ABGR8888);
	//	SDL_LockTexture(texture, nullptr, (void**)&pixelBuffer, &pitch);

	//	surface = SDL_GetWindowSurface(window);
	//	if (surface == nullptr)
	//	{
	//		exit(-1);
	//	}

	//	renderer->SetRenderer(surfaceWidth, surfaceHeight, pixelBuffer);
	//	//renderer = Renderer(surfaceWidth, surfaceHeight, pixelBuffer, mainRenderer, this->texture);


	//	//处理音频设备初始化
	//	if (Mix_OpenAudio(
	//		MIX_DEFAULT_FREQUENCY,
	//		MIX_DEFAULT_FORMAT,
	//		MIX_CHANNELS,
	//		4096
	//	) == -1)
	//	{
	//		Debug("Init audio device failed");
	//		exit(-1);
	//	}

	//}



	//NoaEngineSDL::~NoaEngineSDL() {

	//	Mix_CloseAudio();
	//	SDL_DestroyRenderer(mainRenderer);
	//	SDL_DestroyTexture(texture);
	//	SDL_DestroyWindow(window);
	//	SDL_Quit();
	//}

	//void* NoaEngineSDL::PixelBuffer() {
	//	return pixelBuffer;
	//}

	//int NoaEngineSDL::Run()
	//{
	//	//运行游戏
	//	tp1 = chrono::system_clock::now();
	//	chrono::duration<float> elapsedTime;
	//	tp2 = chrono::system_clock::now();

	//	inputSystem.SetGraphicAPI(GRAPHIC::SDL);

	//	Start();

	//	thread t = thread([this]() {this->EngineThread(); });

	//	EventLoop();

	//	t.join();

	//	Quit();
	//	return 0;
	//}

	//int NoaEngineSDL::Quit()
	//{
	//	OnDisable();
	//	sceneManager.Quit();
	//	isRun = false;
	//	return 0;
	//}

	//void NoaEngineSDL::EngineThread()
	//{
	//	while (isRun)
	//	{

	//		tp2 = chrono::system_clock::now();
	//		elapsedTime = tp2 - tp1;
	//		Time::deltaTime = Time::timeScale * elapsedTime.count();

	//		Time::time += Time::deltaTime;
	//		if (Time::time > 2 * PI)
	//		{
	//			Time::time = 0;
	//		}

	//		renderer->FullScreen(BLACK);

	//		sceneManager.Update();
	//		Update();

	//		SDL_UnlockTexture(texture);
	//		SDL_RenderCopy(mainRenderer, texture, nullptr, nullptr);

	//		for (const auto& instance : spriteSDLInstances)
	//		{
	//			SDL_RenderCopyEx(mainRenderer
	//				, instance.texture
	//				, instance.srcRect
	//				, instance.dstRect
	//				, instance.eulerAngle
	//				, nullptr
	//				, (instance.flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE)
	//			);
	//		}

	//		SDL_RenderPresent(mainRenderer);

	//		spriteSDLInstances.clear();

	//		tp1 = tp2;

	//	}
	//}

	//void NoaEngineSDL::EventLoop()
	//{
	//	//执行游戏主类的update
	//	while (SDL_WaitEvent(&ioEvent)&&isRun)
	//	{
	//		inputSystem.Update();

	//		if (ioEvent.type == SDL_QUIT)
	//		{
	//			Quit();
	//		}
	//	}
	//}

	ThreadPool::ThreadPool(size_t numThreads) : numThreads(numThreads), stop(false) {
		for (size_t i = 0; i < numThreads; ++i) {
			threads.emplace_back(std::bind(&ThreadPool::WorkerThread, this));
		}
	}

	ThreadPool::~ThreadPool() {
		{
			std::unique_lock<std::mutex> lock(queueMutex);
			stop = true;
		}
		condition.notify_all();

		for (std::thread& thread : threads)
		{
			thread.detach();
		}
	}

	void ThreadPool::Enqueue(std::function<void()> task) {
		{
			std::unique_lock<std::mutex> lock(queueMutex);
			tasks.emplace(task);
		}
		condition.notify_one();
	}

	void ThreadPool::WorkerThread() {
		while (true) {
			std::function<void()> task;

			{
				std::unique_lock<std::mutex> lock(queueMutex);
				condition.wait(lock, [this] { return stop || !tasks.empty(); });

				if (stop && tasks.empty()) {
					return;
				}

				task = tasks.front();
				tasks.pop();
			}

			task();  // 执行任务
		}
	}

#pragma endregion


#pragma region OPENGL

	

	NoaEngine::NoaEngine(int width, int height, WindowMode windowMode, string gameName)
	{

		pixelWidth = width;
		pixelHeight = height;
		pixelBuffer = new uint32_t[width * height];

		renderer->SetRenderer(width, height, pixelBuffer);
		platform->Create(width,height,windowMode,gameName);
		window = platform->GetWindow();
		renderer->SetContext(window);
		renderer->InitRenderer();

		texture = renderer->CreateTexture(width,height,pixelBuffer);

		//处理音频设备初始化
		if (Mix_OpenAudio(
			MIX_DEFAULT_FREQUENCY,
			MIX_DEFAULT_FORMAT,
			MIX_CHANNELS,
			4096
		) == -1)
		{
			Debug("Init audio device failed");
			exit(-1);
		}



	}

	NoaEngine::~NoaEngine()
	{
		delete pixelBuffer;
		Mix_CloseAudio();
	}


	int NoaEngine::Run()
	{
		
		Start();
		
		//thread t = thread([this]() {this->EventLoop(); });

		this->EngineThread();

		//t.join();

		Quit();

		return 0;

	}

	void NoaEngine::EngineThread()
	{
		while (!platform->CheckWindowClose())
		{
			tp2 = std::chrono::system_clock::now();
			elapsedTime = tp2 - tp1;
			Time::deltaTime = Time::timeScale * elapsedTime.count();

			Time::time += Time::deltaTime;
			if (Time::time > 2 * PI)
			{
				Time::time = 0;
			}

			while (SDL_PollEvent(&inputSystem.ioEvent))
			{
				inputSystem.Update();
				if (inputSystem.ioEvent.type == SDL_QUIT)
				{
					Quit();
				}
			}

			sceneManager.Update();
			Update();

			int i = 0;
			renderer->Clear();
			texture->UpdateTexture(pixelBuffer, pixelWidth, pixelHeight);
			renderer->DrawTexture(this->texture, i, 0, 0, pixelWidth, pixelHeight);
			i++;
			for (const auto& instance : spriteInstances)
			{
				renderer->DrawTexture(
					instance.texture
					, i
					, instance.position.x
					, instance.position.y
					, instance.scale.x
					, instance.scale.y
					, instance.eulerAngle
					, instance.flip
				);
				i++;
			}

			renderer->Present(window);
			
			spriteInstances.clear();

			tp1 = tp2;

		}
	}

	void NoaEngine::EventLoop()
	{
		
		while (SDL_WaitEvent(&inputSystem.ioEvent))
		{
			inputSystem.Update();
			if (inputSystem.ioEvent.type == SDL_QUIT)
			{
				Quit();
			}
		}
	}

	int NoaEngine::Quit()
	{
		OnDisable();
		sceneManager.Quit();
		platform->Quit();
		return 0;
	}

#pragma endregion

	


	void Debug(std::string msg)
	{
		/*std::time_t current_time = std::time(nullptr);

		char time_string[100];
		std::strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", std::localtime(&current_time));
		cout << "[INFO " << time_string << "]:" << msg << endl;*/

		// 获取当前时间点
		auto current_time = std::chrono::system_clock::now();

		// 将当前时间转换为时间戳
		std::time_t current_time_t = std::chrono::system_clock::to_time_t(current_time);


		// 使用 std::put_time 格式化输出时间
		cout << "[INFO " << current_time_t << "]:" << msg << endl;

	}








}



