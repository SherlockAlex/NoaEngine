#ifndef NOAENGINE_ACTOR_H
#define NOAENGINE_ACTOR_H

#include <vector>
#include "Transform.h"
#include "NObject.h"

/*
* 
* Actor模块
* 作用:游戏场景Scene中的活动的对象，可挂载组件实现各种各样的游戏行为
* 
*/

namespace noa {

	struct Collision;
	typedef struct Collision Collision;

	class Transform;
	class Scene;
	class ActorComponent;
	class Rigidbody;

	class Actor
	{
		friend class NObject<Actor>;
	protected:
		Scene* activeScene = nullptr;
	private:
		bool isActive = true;

		std::vector<ActorComponent*> components;
		Rigidbody* rigidbody = nullptr;

	public:
		Transform transform;
		bool isRaycasted = false;

	protected:
		Actor(Scene* activeScene);
		virtual ~Actor();

		

	public:
		virtual void Delete();

		virtual void Awake() {};
		virtual void OnEnable() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void OnDisable() {}
		virtual void OnDestroy() {}

		virtual void Destroy();

		virtual void SetActive(bool value);
		virtual bool GetActive();
		Scene* GetActiveScene();

		

	public:
		size_t GetHash() const {
			return id;
		}

		template<class T>
		T GetActorAs() {
			return dynamic_cast<T>(this);
		}

		template<class T>
		bool TryGetActorAs(T & outBuffer)
		{
			T buffer = dynamic_cast<T>(this);
			if (buffer == nullptr)
			{
				outBuffer = nullptr;
				return false;
			}
			outBuffer = buffer;
			return true;
		}

		Actor* GetActor() {
			return this;
		}

	public:
		void SetActiveScene(Scene * scene);

		virtual void OnTrigger(Collision collision);
		virtual void OnHitTile() {}

		void AddComponent(ActorComponent* component);
		void ComponentAwake();
		void ComponentOnEnable();
		void ComponentStart();
		void ComponentUpdate();
		void ComponentOnDisable();
		void ComponentOnDestroy();
		void DestroyComponent();

		void AddRigidbody(Rigidbody* rigid);

	private:
		size_t id;
		static size_t nextId;

		static size_t GetNextId() {
			return nextId++;
		}

	};

}



#endif
