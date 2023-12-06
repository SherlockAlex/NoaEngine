#include "Engine.h"
#include "EngineExtension.h"

#ifdef NOA_WINDOWS
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

namespace noa {

#if defined(NOA_WINDOWS)
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

void noa::NoaEngine::AddExtension(noa::EnginExtension* extension)
{
	if (!extension)
	{
		return;
	}
	this->extensions.push_back(extension);

	//保证扩展的唯一性
	std::sort(extensions.begin(), extensions.end());
	auto last = std::unique(extensions.begin(), extensions.end());
	extensions.erase(last, extensions.end());

}

void noa::NoaEngine::RemoveExtension(noa::EnginExtension* extension)
{
	if (!extension)
	{
		return;
	}

	auto it = std::find(extensions.begin(), extensions.end(), extension);
	if (it != extensions.end())
	{
		extensions.erase(it);
	}

}

noa::NoaEngine::NoaEngine(
	int width, int height
	, WindowMode windowMode
	, GraphicsAPI graphics
	, const std::string& gameName
)
{

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

	const Vector<int>& hardwareSize = platform->GetHardwareScreenSize();

	switch (windowMode)
	{
	case noa::WindowMode::FULLSCREEN:
		noa::Screen::width = hardwareSize.x;
		noa::Screen::height = hardwareSize.y;
		break;
	default:
		noa::Screen::width = (width > hardwareSize.x) ? hardwareSize.x : width;
		noa::Screen::height = (height > hardwareSize.y) ? hardwareSize.y : height;
		break;
	}

	Screen::pixelBuffer = new uint32_t[Screen::width * Screen::height];

	renderer->SetRenderer();//初始化渲染器
	platform->Create(Screen::width, Screen::height, windowMode, gameName);
	windowID = platform->GetWindowID();
	renderer->CreateContext(windowID);
	renderer->InitRenderer();
	renderer->InitFontAsset();
	Input::InitInputSystem(platform->GetPlatformEvent());
	audioSystem->InitAudioSubSystem();

	texture = renderer->CreateTexture(Screen::width, Screen::height, Screen::pixelBuffer);

}

noa::NoaEngine::~NoaEngine()
{
	delete[] Screen::pixelBuffer;

	for (auto& extension : extensions)
	{
		extension->Delete(extension);
	}

	delete texture;

}


int noa::NoaEngine::Run()
{

	//初始化游戏图层
	rendererInstanceLayer.push_back(std::vector<SpriteGPUInstance>());
	rendererInstanceLayer.push_back(std::vector<SpriteGPUInstance>());
	rendererInstanceLayer.push_back(std::vector<SpriteGPUInstance>());
	rendererInstanceLayer.push_back(std::vector<SpriteGPUInstance>());

	PhysicsSystem::InitPhysicsSystem();

	Start();
	ExtensionStart();
	this->EngineThread();
	Exit();

	SceneManager::DestroySceneManager();
	return 0;

}

int noa::NoaEngine::Exit()
{
	SceneManager::Quit();
	OnExit();
	ExtensionOnExit();
	return 0;
}

void noa::NoaEngine::ExtensionStart() {
	for (auto& extension : extensions)
	{
		extension->Start();
	}
}

void noa::NoaEngine::ExtensionBeforeUpdate() {
	for (auto& extension : extensions)
	{
		extension->BeforeUpdate();
	}
}

void noa::NoaEngine::ExtensionUpdate() {
	for (auto& extension : extensions)
	{
		extension->Update();
	}
}

void noa::NoaEngine::ExtensionRender() {
	for (auto& extension : extensions)
	{
		extension->Render();
	}
}

void noa::NoaEngine::ExtensionOnExit()
{
	for (auto& extension : extensions)
	{
		extension->OnExit();
	}
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
		BeforeUpdate();
		ExtensionBeforeUpdate();

		SceneManager::Update();
		Update();
		ExtensionUpdate();

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

		Render();
		ExtensionRender();
		renderer->Present(windowID);

		tp1 = tp2;
		//计算休眠时间

		std::chrono::duration<double> targetFrameTime(1.0/noa::Time::maxFPS);
		std::chrono::duration<double> sleepTime =
			targetFrameTime 
			- (std::chrono::system_clock::now() - tp1);
		if (sleepTime.count()>0) 
		{
			const int sleepMilliseconds = static_cast<int>(sleepTime.count()*1000);
			Sleep(sleepMilliseconds);
		}
	}
}



