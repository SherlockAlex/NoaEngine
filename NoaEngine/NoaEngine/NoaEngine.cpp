#include "NoaEngine.h"



#ifdef _WIN64
#include "SDLRenderer.h"
#include "SDLTexture.h"
#include "GLRenderer.h"
#include "GLTexture.h"
#include "Platform_Windows.h"
#include "AudioSystem_SDL.h"
#elif defined(__linux__)
#include "GLRenderer.h"
#include "GLTexture.h"
#include "Platform_Linux.h"
#endif


#include <ctime>
#include <thread>
#include <mutex>
#include <queue>

namespace noa {

#if defined(_WIN64) || defined(_WIN32)
	std::shared_ptr<Platform> platform = std::make_shared<Platform_Windows>();
	std::shared_ptr<AudioSystem> audioSystem = std::make_shared<AudioSystem_SDL>();
	std::shared_ptr<Renderer> renderer = nullptr;
#elif defined(__linux__)
	std::shared_ptr<Platform> platform = std::make_shared<Platform_Linux>();
	std::shared_ptr<AudioSystem> audioSystem = std::make_shared<AudioSystem_SDL>();
	std::shared_ptr<Renderer> renderer = nullptr;
#endif

	std::vector<std::vector<SpriteGPUInstance>> rendererInstanceLayer;
}

noa::NoaEngine::NoaEngine(
	int width, int height
	, WindowMode windowMode
	, GraphicsAPI graphics
	, const std::string& gameName
)
{

	Screen::width = width;
	Screen::height = height;
	Screen::pixelBuffer = new uint32_t[width * height];

	switch (graphics)
	{
	case noa::GraphicsAPI::SDL2:
		renderer = std::make_shared<SDLRenderer>();
		break;
	case noa::GraphicsAPI::OPENGL:
		renderer = std::make_shared<GLRenderer>();
		break;
	default:
		renderer = std::make_shared<SDLRenderer>();
		break;
	}

	renderer->SetRenderer(width, height);
	platform->Create(width, height, windowMode, gameName);
	windowID = platform->GetWindowID();
	renderer->CreateContext(windowID);
	renderer->InitRenderer();
	renderer->InitFontAsset();
	Input::InitInputSystem(platform->GetPlatformEvent());
	audioSystem->InitAudioSubSystem();

	texture = renderer->CreateTexture(width, height, Screen::pixelBuffer);

}

noa::NoaEngine::~NoaEngine()
{
	delete[] Screen::pixelBuffer;
	Mix_CloseAudio();
}


int noa::NoaEngine::Run()
{
	
	//��ʼ����Ϸͼ��
	rendererInstanceLayer.push_back(std::vector<SpriteGPUInstance>());
	rendererInstanceLayer.push_back(std::vector<SpriteGPUInstance>());
	rendererInstanceLayer.push_back(std::vector<SpriteGPUInstance>());
	rendererInstanceLayer.push_back(std::vector<SpriteGPUInstance>());

	Start();
	this->EngineThread();
	Exit();

	return 0;

}

int noa::NoaEngine::Exit()
{
	sceneManager.Quit();
	OnExit();
	return 0;
}

void noa::NoaEngine::EngineThread()
{
	while (!platform->CheckWindowClose())
	{
		tp2 = std::chrono::system_clock::now();
		elapsedTime = tp2 - tp1;
		Time::realDeltaTime = elapsedTime.count();
		Time::deltaTime = Time::timeScale * Time::realDeltaTime;

		Time::time += Time::deltaTime;
		if (Time::time > noa::Math::doublePi)
		{
			Time::time = 0;
			Time::fps = static_cast<int>(1.0f / Time::deltaTime);
		}
		platform->EventLoop();

		sceneManager.Update();
		Update();

		renderer->Clear();
		int textureIndex = 0;
		texture->UpdateTexture(Screen::pixelBuffer, Screen::width, Screen::height);
		renderer->DrawTexture(this->texture, textureIndex, 0, 0, Screen::width, Screen::height, WHITE, 0.0f, false);
		textureIndex++;

		for (auto& layer : rendererInstanceLayer)
		{
			for (const auto& instance : layer)
			{
				renderer->DrawTexture(
				instance.texture
				, textureIndex
				, instance.position.x
				, instance.position.y
				, instance.size.x
				, instance.size.y
				, instance.tint
				, instance.eulerAngle
				, instance.flip
				);
				textureIndex++;
			}
			layer.clear();
		}

		renderer->Present(windowID);

		tp1 = tp2;

	}
}

void noa::Application::Quit() {
	platform->Quit();
}




