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

void noa::ActorComponent::Delete(ActorComponent*& ptr)
{
	delete this;
	if (ptr != nullptr) 
	{
		ptr = nullptr;
	}
}

void noa::ActorComponent::SetActive(bool value)
{
	if (isActive == value)
	{
		return;
	}
	isActive = value;
	if (value)
	{
		OnEnable();
		return;
	}
	OnDisable();
}

bool noa::ActorComponent::GetActive()
{
	return isActive;
}

std::string& noa::ActorComponent::GetActorTag()
{
	return actor->tag;
}

noa::Actor* noa::ActorComponent::GetActor()
{
	return this->actor;
}

void noa::ActorComponent::DeleteActorEvent()
{
	this->actor = nullptr;
}



