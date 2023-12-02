#include "ActorComponent.h"
#include "Actor.h"

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
	ptr = nullptr;
	
}

void noa::ActorComponent::SetActive(bool value)
{
	if (active == value)
	{
		return;
	}
	active = value;
	if (active)
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

std::string& noa::ActorComponent::GetActorTag()
{
	return actor->tag;
}

bool noa::ActorComponent::CompareTag(const std::string& tag)
{
	if (!actor)
	{
		return false;
	}
	return actor->tag == tag;
}

noa::Actor* noa::ActorComponent::GetActor()
{
	return this->actor;
}



