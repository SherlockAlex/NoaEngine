#ifndef NOAENGINE_ACTORCOMPONENT_H
#define NOAENGINE_ACTORCOMPONENT_H

#include <string>
#include "NObject.h"
/*
* ActorPool
* 作用:生成制作Actor及其派生类的对象池，主要用于射击游戏
*/

namespace noa {
	class Actor;

	class ActorComponent:public NOAObject
	{
	protected:
		friend class NObject<ActorComponent>;
		friend void InitNObject(NOAObject* obj);
		friend class Actor;

		Actor* actor = nullptr;
	private:
		bool isActive = true;

	protected:
		ActorComponent(Actor* actor);
		virtual ~ActorComponent();

	protected:

		virtual void Awake() {};
		virtual void OnEnable() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void OnDisable() {}
		virtual void OnDestroy() {}

		void DeleteActorEvent();
	
	private:
		void Delete(ActorComponent*& ptr);

	public:

		virtual void SetActive(bool value);
		virtual bool GetActive();
		std::string& GetActorTag();
		

		Actor* GetActor();

		template<class T>
		T GetActorAs() {
			return dynamic_cast<T>(actor);
		}

		template<class T>
		bool TryGetActorAs(T & out) 
		{
			T buffer = dynamic_cast<T>(actor);
			if (buffer == nullptr)
			{
				out = nullptr;
				return false;
			}
			out = buffer;
			return true;
		}

		template<class T>
		T GetComponentAs() {
			return dynamic_cast<T>(this);
		}

		template<class T>
		bool TryGetComponentAs(T & out) 
		{
			T buffer = dynamic_cast<T>(this);
			if (buffer==nullptr)
			{
				out = nullptr;
				return false;
			}
			out = buffer;
			return true;
		}


	};
}



#endif // !NOAENGINE_ACTORCOMPONENT_H



