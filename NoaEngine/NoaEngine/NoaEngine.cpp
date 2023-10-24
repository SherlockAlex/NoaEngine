#include "NoaEngine.h"
#include <ctime>
#include <thread>
#include <mutex>
#include <queue>

using namespace std;

namespace noa {

#if defined(_WIN64) || defined(_WIN32)||defined(__linux__)
	static shared_ptr<Platform> platform = make_shared<Platform_Windows>();
	shared_ptr<Renderer> renderer = make_shared<GLRenderer>();
#endif

	std::vector<std::vector<SpriteGPUInstance>> rendererInstanceLayer;

#pragma region OPENGL

	

	NoaEngine::NoaEngine(int width, int height, WindowMode windowMode, string gameName)
	{

		Screen::width = width;
		Screen::height = height;
		Screen::pixelBuffer = new uint32_t[width * height];

		renderer->SetRenderer(width, height);
		platform->Create(width,height,windowMode,gameName);
		window = platform->GetWindow();
		renderer->SetContext(window);
		renderer->InitRenderer();
		renderer->InitFontAsset();

		if (windowMode == WindowMode::WINDOW) 
		{
			Screen::hardwareScreenScale.x = width;
			Screen::hardwareScreenScale.y = height;
		}

		texture = renderer->CreateTexture(width,height,Screen::pixelBuffer);

		if (Mix_OpenAudio(
			MIX_DEFAULT_FREQUENCY,
			MIX_DEFAULT_FORMAT,
			MIX_CHANNELS,
			4096
		) == -1)
		{
			//��һЩû���������豸�У������ʼ��ʧ��
			Debug::Error("Init audio device failed");
			Audio::isInitSuccessful = false;
		}
		else {
			Audio::isInitSuccessful = true;
		}
		

		
	}

	NoaEngine::~NoaEngine()
	{
		delete[] Screen::pixelBuffer;
		Mix_CloseAudio();
	}


	int NoaEngine::Run()
	{
		
		rendererInstanceLayer.push_back(std::vector<SpriteGPUInstance>());
		rendererInstanceLayer.push_back(std::vector<SpriteGPUInstance>());
		rendererInstanceLayer.push_back(std::vector<SpriteGPUInstance>());

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
			
			renderer->Clear();
			int textureIndex = 0;
			texture->UpdateTexture(Screen::pixelBuffer, Screen::width, Screen::height);
			renderer->DrawTexture(this->texture, textureIndex, 0, 0, Screen::width, Screen::height,WHITE,0.0f,false);
			textureIndex++;

			for (auto & layer:rendererInstanceLayer)
			{
				for (const auto& instance :layer)
				{
					renderer->DrawTexture(
					instance.texture
					, textureIndex
					, instance.position.x
					, instance.position.y
					, instance.scale.x
					, instance.scale.y
					, instance.tint
					, instance.eulerAngle
					, instance.flip
					);
					textureIndex++;
				}
				layer.clear();
			}

			renderer->Present(window);

			tp1 = tp2;

		}
	}

	void NoaEngine::EventLoop()
	{
		
	}

	int NoaEngine::Quit()
	{
		sceneManager.Quit();
		platform->Quit();
		OnExit();
		return 0;
	}

#pragma endregion

}



