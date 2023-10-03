#include "Actor.h"
#include "NoaEngine.h"
#include "ActorComponent.h"
#include "Physics.h"
#include "SpriteRenderer.h"

#include <queue>
#include <list>

using namespace std;

namespace noa 
{
	void DestroyBehaviour(Actor* behaviour);
}

void noa::DestroyBehaviour(noa::Actor* behaviour)
{
	noa::Scene* activeScene = behaviour->GetActiveScene();
	activeScene->RemoveActor(behaviour);

	noa::Debug("Behaviour " + to_string(behaviour->GetHash()) + " is destroy");
}

size_t noa::Actor::nextId = 0;

noa::Actor::Actor(noa::Scene* activeScene)
{
	id = GetNextId();
	this->activeScene = activeScene;
	if (activeScene != nullptr)
	{
		activeScene->AddActor(this);
	}
}

noa::Actor::~Actor()
{
	if (rigidbody!=nullptr)
	{
		rigidbody->actor = nullptr;
		delete rigidbody;
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
	Debug("Remove actor");
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
	if (rigid == nullptr||rigidbody!=nullptr)
	{
		return;
	}
	this->rigidbody = rigid;
}

noa::Actor* noa::Actor::Create(Scene* activeScene)
{
	return new Actor(activeScene);
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

void noa::Actor::SetActiveScene(noa::Scene* scene)
{
	this->activeScene = scene;
	scene->AddActor(this);
}

void noa::Actor::OnTrigger(noa::Collision collision)
{
}


