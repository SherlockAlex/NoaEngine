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

	uint32_t* pixelBuffer = nullptr;

	Renderer renderer;
	int pitch;

#pragma region SDL

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
		SDL_LockTexture(texture, nullptr, (void**)&pixelBuffer, &pitch);
		//pixelBuffer = new Uint32[width * height];
		//pitch = 3 * 32 * sizeof(Uint32);

		surface = SDL_GetWindowSurface(window);
		if (surface == nullptr)
		{
			exit(-1);
		}

		renderer = Renderer(surfaceWidth, surfaceHeight, pixelBuffer, mainRenderer, nullptr);

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
		SDL_DestroyRenderer(mainRenderer);
		SDL_DestroyTexture(texture);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void* NoaGameEngine::PixelBuffer() {
		return pixelBuffer;
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

#pragma endregion


#pragma region OPENGL

	NoaEngineGL::NoaEngineGL(int width, int height, GameWindowMode windowMode, string gameName)
	{

		//初始化OpenGL
		if (!glfwInit()) {
			Debug("Failed to initialize GLFW");
			exit(-1);
		}

		this->glPixelHeight = height;
		this->glPixelWidth = width;

		pixelWidth = width;
		pixelHeight = height;

		pixelBuffer = new uint32_t[width * height];

		window = glfwCreateWindow(width, height, gameName.c_str(), NULL, NULL);
		if (!window) {
			Debug("Failed to create GLFW window");
			glfwTerminate();
			exit(-1);
		}

		glfwMakeContextCurrent(window);

		if (glewInit() != GLEW_OK) {
			Debug("Failed to initialize GLEW");
			exit(-1);
		}

		renderer = Renderer(width, height, pixelBuffer, nullptr, nullptr);

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

	NoaEngineGL::~NoaEngineGL()
	{
		Mix_CloseAudio();
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	int NoaEngineGL::Run()
	{


		inputSystem.SetGLWindow(this->window);
		glfwSetScrollCallback(window, InputSystem::MouseScrollCallback);
		

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
		glCompileShader(vertexShader);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
		glCompileShader(fragmentShader);

		GLuint shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		GLuint VBO, VAO, EBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, glPixelWidth, glPixelHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBindTexture(GL_TEXTURE_2D, 0);

		

		Start();

		while ((!glfwWindowShouldClose(window))&&isRun) {
			tp2 = std::chrono::system_clock::now();
			elapsedTime = tp2 - tp1;
			deltaTime = elapsedTime.count();

			glClear(GL_COLOR_BUFFER_BIT);

			///修改像素值
			
			//执行游戏主类的update
			inputSystem.Update();

			sceneManager.Update();
			Update();

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->glPixelWidth, this->glPixelHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);

			///

			glUseProgram(shaderProgram);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			glfwSwapBuffers(window);
			glfwPollEvents();

			inputSystem.mouseWheelEventReceived = false;

			tp1 = tp2;
		}

		Quit();

		//释放资源
		glDeleteProgram(shaderProgram);
		glDeleteTextures(1, &textureID);
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		return 0;

	}

	int NoaEngineGL::Quit()
	{
		OnDisable();
		sceneManager.Quit();
		isRun = false;
		return 0;
	}

#pragma endregion

	


	void Debug(string msg)
	{
		std::time_t current_time = std::time(nullptr);

		char time_string[100];
		std::strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", std::localtime(&current_time));
		cout << "[INFO " << time_string << "]:" << msg << endl;
	}








}



