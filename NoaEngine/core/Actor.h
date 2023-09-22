#ifndef NOAENGINE_ACTOR_H
#define NOAENGINE_ACTOR_H

#include <vector>
#include "Transform.h"

namespace noa {
	//游戏脚本类

	struct Collision;
	typedef struct Collision Collision;

	class Transform;
	class Scene;

	class Actor {
	protected:
		Scene* activeScene = nullptr;
	private:
		bool isActive = true;
	public:
		Transform transform;

	protected:
		Actor(Scene* activeScene);
		virtual ~Actor();

	public:
		
		static Actor* Create(Scene* activeScene);
		virtual void Delete();

		virtual void Awake() {};
		virtual void OnEnable() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void OnDisable() {}
		virtual void OnDestroy() {}

		virtual void Destroy();

		void SetActive(bool value);
		bool GetActive();
		Scene* GetActiveScene();

		// 获取哈希值
		size_t GetHash() const {
			return id;
		}

		// 将Behaviour作为其他组件返回
		template<class T>
		T GetActorAs() {
			return dynamic_cast<T>(this);
		}

		Actor* GetActor() {
			return this;
		}

		void SetActiveScene(Scene * scene);

		//触发触发器，基于触发对象other以一个void*类型的指针
		virtual void OnTrigger(Collision collision);

		virtual void OnHitTile() {}

	private:
		size_t id;
		static size_t nextId;

		// 静态函数，用于获取下一个独一无二的哈希值
		static size_t GetNextId() {
			return nextId++;
		}

	};

}



#endif
