#include "NoaEngine.h"
#include <ctime>
#include <thread>
#include <mutex>


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
			exit(0);
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
			exit(0);
		}

		glContext = SDL_GL_CreateContext(window);

		if (!glContext) {
			Debug("OpenGL context creation failed: " + string(SDL_GetError()));
			exit(0);
		}

		//gladLoadGL();

		surfaceWidth = width;
		surfaceHeight = height;

		pixelWidth = surfaceWidth;
		pixelHeight = surfaceHeight;

		//pixelBuffer = new Uint32[pixelWidth * pixelHeight];

		/*glEnable(GL_TEXTURE_2D);
		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, width, height, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();*/

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

		format = SDL_AllocFormat(SDL_PIXELFORMAT_BGR888);
		SDL_LockTexture(texture, nullptr, &pixelBuffer, &pitch);

		surface = SDL_GetWindowSurface(window);
		if (surface == nullptr) 
		{
			exit(0);
		}
		//pixelBuffer = surface->pixels;

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
			exit(0);
		}

		

		//禁用垂直同步
		SDL_GL_SetSwapInterval(0);
	}



	NoaGameEngine::~NoaGameEngine() {
		//delete pixelBuffer;
		
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyRenderer(mainRenderer);
		SDL_DestroyTexture(texture);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void NoaGameEngine::GLRenderTexture()
	{
		/*GLuint textureID;
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelBuffer);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(0, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(width, 0);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(width, height);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(0, height);
		glEnd();

		glDeleteTextures(1, &textureID);*/
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

		const int rigidbodyCount = rigidbodys.size();
		const int behaviourCount = behaviours.size();
		const int animatorCount = animatorList.size();

		Start();

		for (int i = 0; i < behaviours.size(); i++)
		{
			behaviours[i]->Start();
		}
		
		//thread mainThread(mainThreadFunc);
		
		//glClearColor(0, 1, 0, 1);

		while (isRun)
		{
			tp2 = chrono::system_clock::now();
			elapsedTime = tp2 - tp1;
			deltaTime = elapsedTime.count();
			//执行游戏主类的update
			while (SDL_PollEvent(&ioEvent))
			{
				inputSystem.Update();

				if (ioEvent.type == SDL_QUIT)
				{
					Quit();
				}
			}

			for (int i = 0; i < rigidbodyCount; i++)
			{
				rigidbodys[i]->RigidbodyUpdate(deltaTime);
			}

			Update();

			for (int i = 0; i < behaviourCount; i++)
			{
				behaviours[i]->Update();
			}

			for (int i = 0; i < animatorCount; i++)
			{
				animatorList[i]->Update(deltaTime);
			}

			SDL_UnlockTexture(texture);
			SDL_RenderCopy(mainRenderer, texture, nullptr, nullptr);
			SDL_RenderPresent(mainRenderer);
			
			// 更新OpenGL纹理像素数据并渲染
			//UpdateOpenGLTexture();
			//RenderOpenGLTexture();

			//glClear(GL_COLOR_BUFFER_BIT);
			//SDL_GL_SwapWindow(window);

			//SDL_UnlockSurface(surface);
			//SDL_UpdateWindowSurface(window);

			//UpdateOpenGLTexture();
			//RenderOpenGLTexture();

			// 减少内存访问
			const double fps = 1.0 / deltaTime;
			const string windowTitle = gameName + " FPS: " + to_string(fps);
			SDL_SetWindowTitle(window, windowTitle.c_str());

			tp1 = tp2;

		}

		//mainThread.join();
		
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
		isRun = false;
		SDL_Quit();
		return 0;
	}


	void Debug(string msg)
	{
		std::time_t current_time = std::time(nullptr);

		char time_string[100];
		std::strftime(time_string, sizeof(time_string), "%Y-%m-%d %H:%M:%S", std::localtime(&current_time));
		cout << "[INFO " << time_string << "]:" << msg << endl;
	}
}



