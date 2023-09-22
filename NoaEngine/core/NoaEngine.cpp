#include "NoaEngine.h"
#include <ctime>
#include <thread>
#include <mutex>
#include <queue>

using namespace std;

namespace noa {
	extern unordered_map <size_t, Actor*> behaviours;
	extern unordered_map<size_t, Rigidbody*> rigidbodys;

	//mutex mtx; // 定义互斥锁对象

	int pixelHeight = 0;
	int pixelWidth = 0;

	float deltaTime = 0;
	float gameTime = 0;

	Renderer renderer;
	int pitch;

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
			exit(-1);
		}

		//设置opengl
		//SDL_GL_SetAttribute();
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		window = SDL_CreateWindow(
			gameName.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			width,
			height,
			gameWindowMode | SDL_WINDOW_OPENGL
		);
		if (window == nullptr)
		{
			Debug("Create window faild");
			exit(-1);
		}

		glContext = SDL_GL_CreateContext(window);

		surfaceWidth = width;
		surfaceHeight = height;

		pixelWidth = surfaceWidth;
		pixelHeight = surfaceHeight;

		mainRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (mainRenderer == nullptr)
		{
			exit(-1);
		}

		texture = SDL_CreateTexture(mainRenderer,
			SDL_PIXELFORMAT_BGR888,
			SDL_TEXTUREACCESS_STREAMING,
			width,
			height
		);
		if (texture == nullptr) {
			exit(-1);
		}

		format = SDL_AllocFormat(SDL_PIXELFORMAT_BGR888);
		SDL_LockTexture(texture, nullptr, &pixelBuffer, &pitch);

		surface = SDL_GetWindowSurface(window);
		if (surface == nullptr) 
		{
			exit(-1);
		}

		renderer = Renderer(surfaceWidth, surfaceHeight, pixelBuffer,mainRenderer,texture);

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



	NoaGameEngine::~NoaGameEngine() {
		
		Mix_CloseAudio();
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyRenderer(mainRenderer);
		SDL_DestroyTexture(texture);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void NoaGameEngine::GLRenderTexture()
	{

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

		while (isRun)
		{
			tp2 = chrono::system_clock::now();
			elapsedTime = tp2 - tp1;
			deltaTime = elapsedTime.count();
			
			gameTime += deltaTime;
			if (gameTime > 65535)
			{
				gameTime = 0;
			}

			//执行游戏主类的update
			while (SDL_PollEvent(&ioEvent))
			{
				inputSystem.Update();

				if (ioEvent.type == SDL_QUIT)
				{
					Quit();
				}
			}

			sceneManager.Update();
			Update();

			SDL_UnlockTexture(texture);
			SDL_RenderCopy(mainRenderer, texture, nullptr, nullptr);
			SDL_RenderPresent(mainRenderer);

			// 减少内存访问
			const double fps = 1.0 / deltaTime;
			const string windowTitle = move(gameName + " FPS: " + to_string(fps));
			SDL_SetWindowTitle(window, windowTitle.c_str());

			tp1 = tp2;

		}
		
		Quit();
		return 0;
	}
	
	GLuint openGLTexture;
	void NoaGameEngine::UpdateOpenGLTexture()
	{
		
		glGenTextures(1, &openGLTexture);
		glBindTexture(GL_TEXTURE_2D, openGLTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, surface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);

	}

	void NoaGameEngine::RenderOpenGLTexture()
	{
		// 清空屏幕
		glClear(GL_COLOR_BUFFER_BIT);

		// 使用纹理绘制一个矩形
		glBindTexture(GL_TEXTURE_2D, openGLTexture);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2f(-1.0f, -1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex2f(1.0f, -1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex2f(1.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex2f(-1.0f, 1.0f);
		glEnd();
		glBindTexture(GL_TEXTURE_2D, 0);

		// 刷新缓冲区，显示渲染结果
		SDL_GL_SwapWindow(window);
	}

	int NoaGameEngine::Quit()
	{
		OnDisable();
		sceneManager.Quit();
		isRun = false;
		return 0;
	}

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


	void Debug(string msg)
	{
		std::time_t current_time = std::time(nullptr);

		char time_string[100];
		std::strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", std::localtime(&current_time));
		cout << "[INFO " << time_string << "]:" << msg << endl;
	}
}



