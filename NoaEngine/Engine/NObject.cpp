#include "NObject.h"
#include "Actor.h"
#include "ActorComponent.h"

void noa::InitNObject(noa::NoaObject* obj)
{
	//�����ɵ�NoaObject������г�ʼ������
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
