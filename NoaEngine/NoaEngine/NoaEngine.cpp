#include "NoaEngine.h"
#include <ctime>
#include <thread>
#include <mutex>
#include <queue>

using namespace std;

namespace noa {

#ifdef _WIN64
	static shared_ptr<Platform> platform = make_shared<Platform_Windows>();
#endif // _WIN64

#if defined(_WIN64) || defined(_WIN32)||defined(__linux__)
	shared_ptr<Renderer> renderer = make_shared<GLRenderer>();
#endif

	std::vector<SpriteGPUInstance> spriteInstances;

	int pixelHeight = 0;
	int pixelWidth = 0;

	uint32_t* pixelBuffer = nullptr;

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

		if (Mix_OpenAudio(
			MIX_DEFAULT_FREQUENCY,
			MIX_DEFAULT_FORMAT,
			MIX_CHANNELS,
			4096
		) == -1)
		{
			Debug::Error("Init audio device failed");
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

		this->EngineThread();

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

			while (SDL_PollEvent(&inputSystem.e))
			{
				switch (inputSystem.e.type)
				{
				case SDL_QUIT:
					Quit();
					break;
				default:
					inputSystem.Update();
					break;
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
		
		while (SDL_WaitEvent(&inputSystem.e))
		{
			switch (inputSystem.e.type)
			{
			case SDL_QUIT:
				Quit();
				break;
			default:
				inputSystem.Update();
				break;
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

}



