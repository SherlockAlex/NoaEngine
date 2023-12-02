#include "SceneManager.h"
#include "Scene.h"
#include "Actor.h"
#include "Scene.h"
#include "PhysicsSystem.h"

noa::Scene* noa::SceneManager::oldScene = nullptr;
noa::Scene* noa::SceneManager::activeScene = nullptr;
noa::Scene* noa::SceneManager::nextScene = nullptr;

std::map<std::string, noa::Scene*> noa::SceneManager::sceneList;

bool noa::SceneManager::done = true;
bool noa::SceneManager::isLoading = false;
bool noa::SceneManager::isQuit = false;
bool noa::SceneManager::quited = false;

noa::Scene* noa::SceneManager::GetActiveScene()
{
	return activeScene;
}

void noa::SceneManager::LoadScene(const std::string& sceneName)
{

	if (isLoading)
	{
		return;
	}

	if (sceneList.count(sceneName) <= 0)
	{
		Debug::Error("Load scene:" + sceneName + "failed");
		return;
	}

	nextScene = sceneList[sceneName];
	done = false;
	isLoading = true;

	oldScene = activeScene;
	activeScene = nextScene;
	activeScene->SceneChildOnUpdate();
	activeScene->SceneChildOnTick();
	activeScene->SceneChildRender();
	nextScene = nullptr;

	Awake();
	Start();

}

void noa::SceneManager::AddScene(Scene* scene)
{
	if (scene == nullptr)
	{
		return;
	}
	if (activeScene == nullptr)
	{
		activeScene = scene;
	}

	sceneList[scene->name] = scene;

}

void noa::SceneManager::Awake()
{
	if (activeScene != nullptr)
	{
		activeScene->SceneChildOnLoad();
	}
}

void noa::SceneManager::Destroy()
{
	if (activeScene != nullptr)
	{
		oldScene = activeScene;
		activeScene = nullptr;
	}
}

void noa::SceneManager::Start()
{
	if (activeScene != nullptr)
	{
		activeScene->SceneChildOnStart();
	}
}

void noa::SceneManager::Update()
{
	if (activeScene == nullptr)
	{
		return;
	}

	activeScene->ActorUpdate();

	//更新物理系统
	PhysicsSystem::Update(PhysicsSystem::step);

	activeScene->SceneChildOnUpdate();
	activeScene->SceneChildOnTick();
	activeScene->SceneChildRender();

	if (!done && oldScene != nullptr && oldScene != activeScene)
	{
		oldScene->DestoyScene();
		oldScene = nullptr;
	}

	if (!done)
	{
		done = true;
		isLoading = false;
	}

}

void noa::SceneManager::DestroySceneManager() {
	if (!sceneList.empty())
	{
		for (auto& scene : sceneList)
		{
			if (scene.second == nullptr)
			{
				scene.second->Delete(scene.second);
			}
		}
	}
	sceneList.clear();
}

void noa::SceneManager::Quit()
{
	isQuit = true;

}

noa::Actor* noa::SceneManager::FindActorWithTag(const std::string& tag)
{
	if (activeScene == nullptr)
	{
		return nullptr;
	}

	return activeScene->FindActorWithTag(tag);

}

std::vector<noa::Actor*> noa::SceneManager::FindActorsWithTag(const std::string& tag)
{
	if (activeScene == nullptr)
	{
		return std::vector<Actor*>();
	}
	return activeScene->FindActorsWithTag(tag);
}
