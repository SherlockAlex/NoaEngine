#include "Actor.h"
#include "NoaEngine.h"
#include "ActorComponent.h"
#include "Physics.h"
#include "SpriteRenderer.h"

#include <queue>
#include <list>

using namespace std;

size_t noa::Actor::nextId = 0;

noa::Actor::Actor(noa::Scene* activeScene)
{
	if (activeScene == nullptr) 
	{
		Debug::Error("Actor's scene is nullptr");
		exit(-1);
	}
	id = GetNextId();
	this->activeScene = activeScene;

	activeScene->AddActor(this);
}

noa::Actor::~Actor()
{

	for (auto & component:components)
	{
		component->DeleteActorEvent();
	}

	for (auto & instance:spriteRendererInstances) 
	{
		if (instance.actor == this) 
		{
			instance.actor = nullptr;
		}
	}

	DestroyComponent();
	
}

void noa::Actor::SetComponentActive(bool value)
{

	for (auto & component:components) 
	{
		if (component) 
		{
			component->SetActive(value);
		}
	}
}

void noa::Actor::AddComponent(noa::ActorComponent* component)
{
	if (component == nullptr)
	{
		return;
	}
	components.push_back(component);

}

void noa::Actor::ComponentAwake()
{
	for (auto& component : components)
	{
		if (component == nullptr)
		{
			continue;
		}
		component->Awake();
	}

}

void noa::Actor::ComponentOnEnable()
{
	for (auto& component:components)
	{
		if (component == nullptr)
		{
			continue;
		}
		component->OnEnable();
	}
}

void noa::Actor::ComponentStart()
{

	for (auto& component : components)
	{
		if (component == nullptr)
		{
			continue;
		}
		component->Start();
	}

}

void noa::Actor::ComponentUpdate()
{

	for (auto& component : components)
	{
		if (component == nullptr||!component->GetActive())
		{
			continue;
		}
		component->Update();
	}

}

void noa::Actor::ComponentOnDisable()
{
	for (auto& component : components)
	{
		if (component == nullptr)
		{
			continue;
		}
		component->OnDisable();
	}

}

void noa::Actor::ComponentOnDestroy()
{

	for (auto& component : components)
	{
		if (component == nullptr)
		{
			continue;
		}
		component->OnDestroy();
	}

}

void noa::Actor::DestroyComponent()
{
	if (components.empty())
	{
		return;
	}

	std::sort(components.begin(), components.end());
	auto last = std::unique(components.begin(), components.end());
	components.erase(last, components.end());

	for (auto& component : components)
	{
		if (component == nullptr)
		{
			continue;
		}
		component->OnDestroy();
		component->Delete(component);
	}

	components.clear();
}


void noa::Actor::Delete(Actor *& ptr)
{
	delete this;
	if (ptr !=nullptr)
	{
		ptr = nullptr;
	}
}

void noa::Actor::Destroy()
{
	SetActive(false);
	ComponentOnDestroy();
	OnDestroy();
	isRemoved = true;
}

void noa::Actor::SetActive(bool value)
{
	if (isActive == value)
	{
		return;
	}
	SetComponentActive(value);
	isActive = value;
	
	if (isActive)
	{	
		OnEnable();
		return;
	}
	OnDisable();

}

bool noa::Actor::GetActive()
{
	return isActive;
}

noa::Scene* noa::Actor::GetActiveScene()
{
	return activeScene;
}

noa::Actor* noa::Actor::HandleActor(NOAObject* object)
{
	return dynamic_cast<Actor*>(object);
}

void noa::Actor::SetActiveScene(noa::Scene* scene)
{
	this->activeScene = scene;
	scene->AddActor(this);
}

void noa::Actor::OnTrigger(const noa::Collision & collision)
{

}


