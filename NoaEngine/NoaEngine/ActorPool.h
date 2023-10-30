#ifndef NOAENGINE_ACTORPOOL_H
#define NOAENGINE_ACTORPOOL_H

#include "ObjectPool.h"
#include "Actor.h"

/*
* ActorPool
* 作用:生成制作Actor及其派生类的对象池，主要用于射击游戏
*/

namespace noa {

	template<class T>
	class ActorPool:public ObjectPool<T>
	{
	protected:
		ActorPool() {}
		virtual ~ActorPool() {}

	public:

		virtual void Prewarm(int number) override 
		{
			ObjectPool<T>::Prewarm(number);
		}

		virtual T* Request() override {
			T* member = ObjectPool<T>::Request();
			Actor* actor = Actor::HandleActor((NOAObject*)member);
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
			Actor* actor = Actor::HandleActor((NOAObject*)member);
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