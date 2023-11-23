#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "Resource.h"
#include "Actor.h"
#include "Scene.h"
#include "PhysicsSystem.h"
#include "Camera.h"
#include "Debug.h"

using namespace std;

noa::SceneManager noa::sceneManager;

noa::Scene::Scene(const std::string& name)
{
	this->name = name;
	actors.reserve(1024 * 10);
}

noa::Scene::~Scene()
{
	DestoyScene();
}

noa::Scene* noa::Scene::CreateChild(const std::string& name) 
{
	noa::Scene* child = new noa::Scene(name);
	this->AddSceneChild(child);
	return child;
}

void noa::Scene::AddSceneChild(noa::Scene* child) 
{
	if (!child) 
	{
		return;
	}
	this->sceneChildren[child->GetName()] = child;
}

void noa::Scene::ActiveSceneChild(const std::string& name) 
{
	if (sceneChildren.count(name)<=0)
	{
		return;
	}
	this->sceneStack.push(sceneChildren.at(name));
}

void noa::Scene::CloseSceneChild() {
	if (this->sceneStack.empty()) 
	{
		return;
	}
	this->sceneStack.pop();
}

void noa::Scene::AddActor(Actor* actor)
{
	if (actor == nullptr)
	{
		return;
	}
	actors.push_back(actor);

}

void noa::Scene::ActorUpdate()
{

	if (!sceneStack.empty())
	{
		//子场景Update
		sceneStack.top()->ActorUpdate();
		return;
	}

	for (const auto& actor : actors)
	{
		if (actor == nullptr 
			|| !actor->GetActive() 
			|| actor->isRemoved)
		{
			continue;
		}
		actor->ComponentUpdate();
		actor->Update();
	}

	for (const auto& actor : actors)
	{
		if (actor == nullptr 
			|| !actor->GetActive() 
			|| actor->isRemoved)
		{
			continue;
		}
		actor->LateUpdate();
		actor->ComponentLateUpdate();
	}

	

}

void noa::Scene::DestoyScene()
{
	std::sort(actors.begin(), actors.end());
	auto actorLast = std::unique(actors.begin(), actors.end());
	actors.erase(actorLast, actors.end());

	for (auto& actor : actors)
	{
		if (actor == nullptr)
		{
			continue;
		}
		actor->Delete(actor);
	}

	actors.clear();

	while (!sceneStack.empty())
	{
		sceneStack.pop();
	}

	if (!sceneChildren.empty())
	{
		for (auto& scene : sceneChildren)
		{
			if (scene.second != nullptr)
			{
				scene.second->onUnload(scene.second);
				scene.second->DestoyScene();
				scene.second->Delete(scene.second);
			}
		}
	}
	sceneChildren.clear();
	this->onUnload(this);
}

void noa::Scene::Delete(noa::Scene*& ptr)
{
	ptr = nullptr;
	delete this;
}

void noa::Scene::SceneChildOnLoad() {
	for (auto& child : sceneChildren)
	{
		if (child.second)
		{
			child.second->onLoad(child.second);
		}
	}

	this->onLoad(this);

}

void noa::Scene::SceneChildOnStart() {
	for (auto& child:sceneChildren) 
	{
		if (child.second)
		{
			child.second->onStart(child.second);
		}
	}

	this->onStart(this);

}

void noa::Scene::SceneChildRender() {

	for (auto& child : sceneChildren)
	{
		for (const auto& actor : child.second->actors)
		{
			if (actor == nullptr 
				|| !actor->GetActive() 
				|| actor->isRemoved)
			{
				continue;
			}
			actor->Render();
			actor->ComponentRender();
		}
	}

	for (const auto& actor : actors)
	{
		if (actor == nullptr
			|| !actor->GetActive()
			|| actor->isRemoved)
		{
			continue;
		}
		actor->Render();
		actor->ComponentRender();
	}

}

void noa::Scene::SceneChildOnUpdate() 
{

	if (sceneStack.empty()) 
	{
		onUpdate.Invoke(this);
		return;
	}
	noa::Scene* scene = sceneStack.top();
	scene->onUpdate(scene);

}

void noa::Scene::SceneChildOnTick() {

	for (auto& child:sceneChildren) 
	{
		if (child.second) 
		{
			child.second->SceneChildOnTick();
		}
		
	}

	onTick.Invoke(this);

}

noa::Actor* noa::Scene::FindActorWithTag(const string& tag)
{
	noa::Actor* buffer = nullptr;
	for (auto& actor : actors)
	{
		if (actor == nullptr 
			|| !actor->GetActive() 
			|| actor->isRemoved)
		{
			continue;
		}
		if (actor->tag == tag)
		{
			buffer = actor;
			break;
		}
	}

	if (buffer)
	{
		return buffer;
	}

	for (auto& child:sceneChildren) 
	{
		if (child.second) 
		{
			buffer = child.second->FindActorWithTag(tag);
		}
	}

	return buffer;
}

std::vector<noa::Actor*> noa::Scene::FindActorsWithTag(const std::string& tag)
{
	std::vector<Actor*> target;

	for (auto& actor : this->actors)
	{
		if (actor == nullptr 
			|| actor->isRemoved 
			|| !actor->GetActive() 
			|| actor->tag != tag)
		{
			continue;
		}
		target.push_back(actor);
	}

	for (auto& child:sceneChildren) 
	{
		if (!child.second)
		{
			continue;
		}

		std::vector<Actor*> childTarget = 
			child.second->FindActorsWithTag(tag);

		for (auto& actor:childTarget) 
		{
			target.push_back(actor);
		}

	}

	return target;
}

std::string noa::Scene::GetName()
{
	return this->name;
}

noa::Scene* noa::SceneManager::CreateScene(const std::string& name)
{
	noa::Scene* scene = new Scene(name);
	sceneManager.AddScene(scene);
	return scene;
}

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

	if (this->sceneList.count(sceneName) <= 0)
	{
		Debug::Error("Load scene:" + sceneName + "failed");
		return;
	}

	nextScene = sceneList[sceneName];
	done = false;
	isLoading = true;

	oldScene = activeScene;
	activeScene = nextScene;
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
	activeScene->SceneChildRender();

	//更新物理系统
	PhysicsSystem::Update(PhysicsSystem::step);

	activeScene->SceneChildOnUpdate();
	activeScene->SceneChildOnTick();

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

noa::SceneManager::~SceneManager()
{
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
	if (this->activeScene == nullptr)
	{
		return nullptr;
	}
	
	return activeScene->FindActorWithTag(tag);
	
}

std::vector<noa::Actor*> noa::SceneManager::FindActorsWithTag(const std::string& tag)
{
	if (this->activeScene == nullptr)
	{
		return std::vector<Actor*>();
	}
	return this->activeScene->FindActorsWithTag(tag);
}
