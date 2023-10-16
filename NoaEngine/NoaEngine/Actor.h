#ifndef NOAENGINE_ACTOR_H
#define NOAENGINE_ACTOR_H

#include <vector>

#include "Transform.h"
#include "NObject.h"

/*
* Actor模块
* 作用:游戏场景Scene中的活动的对象，可挂载组件实现各种各样的游戏行为
*/

namespace noa {

	struct Collision;
	typedef struct Collision Collision;

	class Transform;
	class Scene;
	class ActorComponent;

	class Actor:public NOAObject
	{
	private:
		friend class NObject<Actor>;
		friend class Scene;
		friend class SceneManager;
		friend class Collider2D;
		friend void InitNObject(NOAObject* obj);

	public:
		std::string tag = "default";
	protected:
		Scene* activeScene = nullptr;
	private:
		bool active = true;

		std::vector<ActorComponent*> components;
	public:
		Transform transform;
		bool isRaycasted = false;

	protected:
		Actor(Scene* activeScene);
		virtual ~Actor();

	protected:
		
		virtual void Awake() {};
		virtual void OnEnable() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void OnDisable() {}
		virtual void OnDestroy() {}

		virtual void OnTrigger(const Collision& collision);
		virtual void OnHitTile() {}


	public:
		void SetActiveScene(Scene* scene);

		void AddComponent(ActorComponent* component);
		virtual void Destroy();

		virtual void SetActive(bool value);
		virtual bool GetActive();
		Scene* GetActiveScene();

		static Actor* HandleActor(NOAObject* object);
		
		bool CompareTag(const std::string & tag);

	public:

		template<class T>
		T* GetActorAs() {
			return dynamic_cast<T*>(this);
		}

		template<class T>
		bool TryGetActorAs(T *& outBuffer)
		{
			T* buffer = dynamic_cast<T*>(this);
			if (buffer == nullptr)
			{
				outBuffer = nullptr;
				return false;
			}
			outBuffer = buffer;
			return true;
		}

		template<class T>
		T* GetComponent() {
			T* buffer = nullptr;
			for (auto & component:components) 
			{
				if (component==nullptr) 
				{
					continue;
				}
				buffer = dynamic_cast<T*>(component);
				if (buffer == nullptr)
				{
					continue;
				}
				break;
			}
			return buffer;
		}

		Actor* FindActorWithTag(const std::string& tag);
		std::vector<Actor*> FindActorsWithTag(const std::string& tag);

		Actor* GetActor();

	private:

		void ComponentAwake();
		void ComponentOnEnable();
		void ComponentStart();
		void ComponentUpdate();
		void ComponentOnDisable();
		void ComponentOnDestroy();
		void DestroyComponent();

		void SetComponentActive(bool value);
		void Delete(Actor*& ptr);

	private:

		bool isRemoved = false;

	};

}



#endif
