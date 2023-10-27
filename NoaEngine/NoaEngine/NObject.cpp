#include "NObject.h"
#include "Actor.h"
#include "ActorComponent.h"

void noa::InitNObject(noa::NOAObject* obj)
{
	noa::Actor* actor = dynamic_cast<noa::Actor*>(obj);
	if (actor != nullptr)
	{
		actor->Awake();
		actor->ComponentAwake();

		if (actor->GetActive())
		{
			actor->ComponentOnEnable();
			actor->OnEnable();

			actor->ComponentStart();
			actor->Start();

		}

	}
}
