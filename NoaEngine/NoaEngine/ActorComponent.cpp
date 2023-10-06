#include "ActorComponent.h"
#include "Actor.h"
#include "NoaEngine.h"

using namespace std;

noa::ActorComponent::ActorComponent(noa::Actor* actor)
{
	actor->AddComponent(this);
	this->actor = actor;
}

noa::ActorComponent::~ActorComponent()
{
	
}

void noa::ActorComponent::Delete()
{
	delete this;
}

void noa::ActorComponent::SetActive(bool value)
{
	if (active == value) 
	{
		return;
	}
	active = value;
	if (value)
	{
		OnEnable();
		return;
	}
	OnDisable();
}

bool noa::ActorComponent::GetActive()
{
	return active;
}

void noa::ActorComponent::DeleteActorEvent()
{
	this->actor = nullptr;
}



