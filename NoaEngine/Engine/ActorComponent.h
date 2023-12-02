#ifndef NOAENGINE_ACTORCOMPONENT_H
#define NOAENGINE_ACTORCOMPONENT_H

#include <string>
#include <functional>
#include "Actor.h"
#include "NObject.h"
#include "Core.h"
/*
* ActorPool
* 作用:生成制作Actor及其派生类的对象池，主要用于射击游戏
*/

#define ACTOR_COMPONENT(T) template<class T> friend class noa::NObject;

namespace noa {
	class NOA_API Actor;

	class NOA_API ActorComponent:public NoaObject
	{
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

		virtual void Render() {};
	
	private:
		void Delete(ActorComponent*& ptr);

	public:

		void SetActive(bool value);
		bool GetActive();
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
		T* GetThisComponentAs() {
			return dynamic_cast<T*>(this);
		}

		template<class T>
		bool TryGetThisComponentAs(std::function<void(T&)> callback)
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

		Actor* FindActorWithTag(const std::string & tag)
		{
			return SceneManager::FindActorWithTag(tag);
		}

		std::vector<Actor*> FindActorsWithTag(const std::string & tag)
		{
			return SceneManager::FindActorsWithTag(tag);
		}

		template<class T>
		T* FindActorWithType() {
			return SceneManager::FindActorWithType<T>();
		}

		template<class T>
		std::vector<T*> FindActorsWithType() {
			return SceneManager::FindActorsWithType<T>();
		}

	protected:
		friend class NObject<ActorComponent>;
		friend void InitNObject(NoaObject* obj);
		friend class Actor;

	private:
		Actor* actor = nullptr;
		bool active = true;

	};
}



#endif // !NOAENGINE_ACTORCOMPONENT_H



