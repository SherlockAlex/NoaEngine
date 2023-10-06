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

	// 这个时候内存只构建了Actor，它的子类还没有被构建
	activeScene->AddActor(this);
}

noa::Actor::~Actor()
{

	if (rigidbody!=nullptr)
	{
		rigidbody->actor = nullptr;
		delete rigidbody;
		rigidbody = nullptr;
	}

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

	if (rigidbody)
	{
		rigidbody->SetActive(value);
	}

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
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i] == nullptr)
		{
			continue;
		}
		components[i]->Awake();
	}
}

void noa::Actor::ComponentOnEnable()
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i] == nullptr)
		{
			continue;
		}
		components[i]->OnEnable();
	}
}

void noa::Actor::ComponentStart()
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i] == nullptr)
		{
			continue;
		}
		components[i]->Start();
	}
}

void noa::Actor::ComponentUpdate()
{
	if (rigidbody!=nullptr&&!rigidbody->isRemoved)
	{
		PhysicsSystem::rigidbodys.push_back(rigidbody);
	}

	for (int i = 0; i < components.size(); i++)
	{
		if (components[i] == nullptr || !components[i]->GetActive())
		{
			continue;
		}
		components[i]->Update();
	}
}

void noa::Actor::ComponentOnDisable()
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i] == nullptr)
		{
			continue;
		}
		components[i]->OnDisable();
	}
}

void noa::Actor::ComponentOnDestroy()
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i] == nullptr)
		{
			continue;
		}
		components[i]->OnDestroy();
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

	for (int i = 0; i < components.size(); i++)
	{
		if (components[i] == nullptr)
		{
			continue;
		}
		components[i]->OnDestroy();
		components[i]->Delete();
	}
	components.clear();
}

void noa::Actor::AddRigidbody(Rigidbody* rigid)
{
	if (rigid == nullptr)
	{
		return;
	}
	if (rigidbody != nullptr)
	{
		delete rigidbody;
	}
	this->rigidbody = rigid;
}

void noa::Actor::Delete()
{
	delete this;
}

void noa::Actor::Destroy()
{
	SetActive(false);
	
	activeScene->RemoveActor(this);
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


