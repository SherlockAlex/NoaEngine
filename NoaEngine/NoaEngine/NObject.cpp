#include "NObject.h"
#include "Actor.h"
#include "ActorComponent.h"

void noa::InitNObject(noa::NoaObject* obj)
{
	//对生成的NoaObject对象进行初始化操作
	noa::Actor* actor = dynamic_cast<noa::Actor*>(obj);
	if (actor)
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
