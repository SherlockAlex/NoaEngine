#ifndef NOAENGINE_ACTORCOMPONENT_H
#define NOAENGINE_ACTORCOMPONENT_H

#include <string>
#include <functional>
#include "Actor.h"
#include "NObject.h"
/*
* ActorPool
* ����:��������Actor����������Ķ���أ���Ҫ���������Ϸ
*/

namespace noa {
	class Actor;

	class ActorComponent:public NOAObject
	{
	protected:
		friend class NObject<ActorComponent>;
		friend void InitNObject(NOAObject* obj);
		friend class Actor;

	private:
		Actor* actor = nullptr;
		bool active = true;

	protected:
		ActorComponent(Actor* actor);
		virtual ~ActorComponent();

	protected:
		virtual void Awake() {};
		virtual void OnEnable() {};
		virtual void Start() {};
		virtual void Update() {};
		virtual void LateUpdate() {};
		virtual void OnDisable() {};
		virtual void OnDestroy() {};
	
	private:
		void Delete(ActorComponent*& ptr);

	public:

		virtual void SetActive(bool value);
		virtual bool GetActive();
		std::string& GetActorTag();
		bool CompareTag(const std::string& tag);

		Actor* GetActor();

		template<class T>
		T* GetActorAs() {
			return dynamic_cast<T*>(actor);
		}

		template<class T>
		bool TryGetActorAs(std::function<void(T&)> callback) 
		{
			T* buffer = dynamic_cast<T*>(actor);
			if (buffer)
			{
				return false;
			}
			callback(*buffer);
			return true;
		}

		template<class T>
		T* GetComponentAs() {
			return dynamic_cast<T*>(this);
		}

		template<class T>
		bool TryGetComponentAs(std::function<void(T&)> callback)
		{
			T* buffer = dynamic_cast<T*>(this);
			if (!buffer)
			{
				return false;
			}
			callback(*buffer);
			return true;
		}

		template<class T>
		T* GetComponent() {
			if (!actor) 
			{
				return nullptr;
			}
			return actor->GetComponent<T>();
		}

		template<class T>
		bool TryGetComponent(std::function<void(T&)> callback)
		{
			if (!actor)
			{
				return false;
			}
			return actor->TryGetComponent<T>(callback);
		}


	};
}



#endif // !NOAENGINE_ACTORCOMPONENT_H



