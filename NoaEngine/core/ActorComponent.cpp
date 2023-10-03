#include "ActorComponent.h"
#include "Actor.h"
#include "NoaEngine.h"

using namespace std;

namespace noa {

	
}

noa::ActorComponent::ActorComponent(noa::Actor* actor)
{
	actor->AddComponent(this);
	this->actor = actor;
}

noa::ActorComponent::~ActorComponent()
{
	noa::Debug("Remove component");
}

noa::ActorComponent* noa::ActorComponent::Create(noa::Actor* actor)
{
	return new noa::ActorComponent(actor);
}

void noa::ActorComponent::Delete()
{
	delete this;
}

void noa::ActorComponent::SetActive(bool value)
{
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



