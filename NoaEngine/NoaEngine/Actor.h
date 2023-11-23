#ifndef NOAENGINE_ACTOR_H
#define NOAENGINE_ACTOR_H

#include <vector>
#include <functional>

#include "Transform.h"
#include "NObject.h"
#include "Scene.h"

/*
* Actor模块
* 作用:游戏场景Scene中的活动的对象，可挂载组件实现各种各样的游戏行为
* 类似Unity中的GameObject
* 
* Actor时游戏的基本单元，Camera，是Actor
* 
* 同时Actor可以根据不同的ActorComponent实现不同的功能
* 
*/

namespace noa {

	class Collider2D;
	class Transform;
	class Scene;
	class ActorComponent;

#define ACTOR(T) template<class T> friend class noa::NObject;

	class Actor:public NoaObject
	{
	protected:
		Actor(Scene* activeScene);
		virtual ~Actor();

	protected:
		
		virtual void Awake() {};
		virtual void OnEnable() {};
		virtual void Start() {};
		virtual void Update() {};
		virtual void LateUpdate() {};
		virtual void OnDisable() {};
		virtual void OnDestroy() {};

		virtual void Render() {};

		virtual void OnTriggerEnter(noa::Collider2D& other);
		virtual void OnHitTile() {}

		virtual void OnMouseHold() {};


	public:
		void SetActiveScene(Scene* scene);

		void AddComponent(ActorComponent* component);
		virtual void Destroy();

		virtual void SetActive(bool value);
		virtual bool GetActive();
		Scene* GetActiveScene();

		static Actor* HandleActor(NoaObject* object);
		
		bool CompareTag(const std::string & tag);

	public:

		template<class T>
		T* GetComponent() {
			T* buffer = nullptr;
			for (auto & component:components) 
			{
				buffer = dynamic_cast<T*>(component);
				if (buffer != nullptr)
				{
					break;
				}
				
			}
			return buffer;
		}

		template<class T>
		bool TryGetComponent(std::function<void(T&)> callback)
		{
			//尝试获取T类型的组件
			//如果没有获取到返回false
			//否则返回True，并执行回调函数
			T* buffer = nullptr;
			for (auto & component:components) 
			{
				buffer = dynamic_cast<T*>(component);
				if (buffer!=nullptr) 
				{
					break;
				}
			}

			if (!buffer)
			{
				return false;
			}

			callback(*buffer);
			return true;



		}

		Actor* GetActor();

		template<class T>
		T* GetActorAs() {
			return dynamic_cast<T*>(this);
		}

		template<class T>
		bool TryGetActorAs(std::function<void(T&)> callback)
		{
			T* buffer = dynamic_cast<T*>(this);
			if (!buffer)
			{
				return false;
			}
			callback(*buffer);
			return true;
		}

		Actor * FindActorWithTag(const std::string & tag)
		{
			return sceneManager.FindActorWithTag(tag);
		}

		std::vector<Actor*> FindActorsWithTag(const std::string& tag) {
			return sceneManager.FindActorsWithTag(tag);
		}

		template<class T>
		T* FindActorWithType() {
			return sceneManager.FindActorWithType<T>();
		}

		template<class T>
		std::vector<T*> FindActorsWithType() {
			return sceneManager.FindActorsWithType<T>();
		}

	private:

		void ComponentAwake();
		void ComponentOnEnable();
		void ComponentStart();
		void ComponentUpdate();
		void ComponentLateUpdate();
		void ComponentOnDisable();
		void ComponentOnDestroy();
		void DestroyComponent();

		void ComponentRender();

		void SetComponentActive(bool value);
		void Delete(Actor*& ptr);

	private:

		bool isRemoved = false;
		bool active = true;
		bool start = false;
		Scene* activeScene = nullptr;

		std::vector<ActorComponent*> components;

		friend class NObject<Actor>;
		friend class Scene;
		friend class SceneManager;
		friend class Collider2D;
		friend class TileCollider2D;
		friend void InitNObject(NoaObject* obj);
	public:
		std::string tag = "default";
		Transform transform;
		bool isRaycasted = false;
	};

}



#endif
