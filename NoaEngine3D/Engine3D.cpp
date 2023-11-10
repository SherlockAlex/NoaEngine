#include "Engine3D.h"
#include "SDLHelper.h"
#include "Debug.h"
#include "GLHelper.h"
#include "GLShader.h"

noa::noa3d::Engine3D::Engine3D()
{
	
	const int init = SDL_Init(SDL_INIT_EVERYTHING);
	if (init != 0)
	{
		Debug::Error("Game init failed");
		exit(-1);
	}

	SDL_Window* window = SDL_CreateWindow(
		"Game"
		, SDL_WINDOWPOS_CENTERED
		, SDL_WINDOWPOS_CENTERED
		,1920/2
		,1080/2
		,SDL_WINDOW_SHOWN|SDL_WINDOW_OPENGL
	);

	if (!window)
	{
		noa::Debug::Error("创建窗口失败");
		exit(-1);
	}

	this->windowID = SDL_GetWindowID(window);

}

noa::noa3d::Engine3D::~Engine3D()
{
}

int noa::noa3d::Engine3D::Run()
{
	SDL_Event e;
	SDL_Window* window = SDL_GetWindowFromID(this->windowID);

	context = (void*)SDL_GL_CreateContext(window);
	if (context == nullptr)
	{
		noa::Debug::Error("创建GL上下文失败");
		exit(-1);
	}

	//初始化OpenGL
	if (glewInit() != GLEW_OK)
	{
		noa::Debug::Error("初始化OpenGL失败");
		exit(-1);
	}

	auto t1 = std::chrono::system_clock::now();
	auto t2 = std::chrono::system_clock::now();
	std::chrono::duration<float> elapsedTime = t2 - t1;
	
	defaultShader.Compile();
	beginAction();

	while (true)
	{
		elapsedTime = t2 - t1;

		while(SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				SDL_Quit();
				break;
			default:
				break;
			}
		}

		this->tickAction();

		//实现代码逻辑
		SDL_GL_SwapWindow(window);

		t1 = t2;
	}

	return 0;
}
