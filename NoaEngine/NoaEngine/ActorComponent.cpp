#include "ActorComponent.h"
#include "Actor.h"

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

std::string& noa::ActorComponent::GetActorTag()
{
	return actor->tag;
}

bool noa::ActorComponent::CompareTag(const std::string& tag)
{
	return actor->tag == tag;
}

noa::Actor* noa::ActorComponent::GetActor()
{
	return this->actor;
}



