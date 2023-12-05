#include "Actor.h"
#include "Debug.h"
#include "ActorComponent.h"
#include "SpriteRenderer.h"
#include "Collider2D.h"


noa::Actor::Actor(noa::Scene* activeScene)
{
	this->activeScene = activeScene;
	activeScene->AddActor(this);
	this->components.reserve(50);
}

noa::Actor::~Actor()
{

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
	if (!component)
	{
		return;
	}
	components.push_back(component);
}

noa::Actor* noa::Actor::GetActor() {
	return this;
}

void noa::Actor::ComponentAwake()
{
	for (auto& component : components)
	{
		if (component == nullptr||component->isRemoved)
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
		if (component == nullptr || component->isRemoved)
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
		if (component == nullptr || component->isRemoved)
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
		if (component == nullptr
			|| component->isRemoved
			||!component->GetActive())
		{
			continue;
		}
		component->Update();
	}
}

void noa::Actor::ComponentLateUpdate()
{
	for (auto& component : components)
	{
		if (component == nullptr 
			|| component->isRemoved
			|| !component->GetActive())
		{
			continue;
		}
		component->LateUpdate();
	}
}

void noa::Actor::ComponentOnDisable()
{
	for (auto& component : components)
	{
		if (component == nullptr
			|| component->isRemoved
			)
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
		if (component == nullptr|| component->isRemoved)
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
		component->isRemoved = true;
		component->OnDestroy();
		component->Delete(component);
	}

	components.clear();
}

void noa::Actor::ComponentRender() {
	for (auto& component : components)
	{
		if (component == nullptr 
			|| !component->GetActive())
		{
			continue;
		}
		component->Render();
	}
}


void noa::Actor::Delete(Actor *& ptr)
{
	ptr = nullptr;
	delete this;
}

void noa::Actor::Destroy()
{
	SetActive(false);
	ComponentOnDestroy();
	OnDestroy();
	isRemoved = true;
}

void noa::Actor::Destroy(noa::ActorComponent* component)
{
	if (!component)
	{
		return;
	}
	component->SetActive(false);
	component->OnDestroy();
	component->isRemoved = true;
}

void noa::Actor::SetActive(bool value)
{
	SetComponentActive(value);
	if (active == value)
	{
		return;
	}
	active = value;
	
	if (!active)
	{	
		OnDisable();
		return;
	}

	OnEnable();
	if (!start)
	{
		ComponentStart();
		Start();
		start = true;
	}

}

bool noa::Actor::GetActive()
{
	return active;
}

noa::Scene* noa::Actor::GetActiveScene()
{
	return activeScene;
}

noa::Actor* noa::Actor::HandleActor(NoaObject* object)
{
	return dynamic_cast<Actor*>(object);
}

bool noa::Actor::CompareTag(const std::string& tag)
{
	return this->tag == tag;
}

void noa::Actor::OnTriggerEnter(noa::Collider2D& other) 
{

}


