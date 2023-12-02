#ifndef NOAENGINE_ACTORPOOL_H
#define NOAENGINE_ACTORPOOL_H

#include "ObjectPool.h"
#include "Actor.h"
#include "Core.h"

/*
* ActorPool
* ����:��������Actor����������Ķ���أ���Ҫ���������Ϸ
*/

namespace noa {

	template<class T>
	class NOA_API ActorPool:public ObjectPool<T>
	{
	public:
		ActorPool() {}
		virtual ~ActorPool() {}

	public:

		virtual void Prewarm(int number) override 
		{
			ObjectPool<T>::Prewarm(number);
		}

		virtual T* Request() override {
			T* member = ObjectPool<T>::Request();
			Actor* actor = Actor::HandleActor((NoaObject*)member);
			if (actor !=nullptr)
			{
				actor->SetActive(true);
				return member;
			}

			Debug::Error("This is not Actor object in ActorPool");
			return nullptr;

		}

		virtual void Return(T* member) override
		{
			Actor* actor = dynamic_cast<Actor*>(member);
			//Actor* actor = Actor::HandleActor((NoaObject*)member);
			if (actor == nullptr)
			{
				Debug::Error("This member is't Actor");
				return;
			}

			actor->SetActive(false);
			ObjectPool<T>::Return(member);

			

		}

	};
}

#endif // !NOAENGINE_ACTORPOOL_H