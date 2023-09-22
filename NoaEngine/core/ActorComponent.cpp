#include "ActorComponent.h"
#include "Actor.h"
#include "NoaEngine.h"

using namespace std;

namespace noa {

	ActorComponent::ActorComponent(Actor* actor)
	{
		actor->AddComponent(this);
	}
	ActorComponent::~ActorComponent()
	{
		Debug("Remove component");
	}
	ActorComponent* ActorComponent::Create(Actor* actor)
	{
		return new ActorComponent(actor);
	}
	void ActorComponent::Delete()
	{
		delete this;
	}
}


