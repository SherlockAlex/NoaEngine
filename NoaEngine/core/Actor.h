#ifndef NOAENGINE_ACTOR_H
#define NOAENGINE_ACTOR_H

#include <vector>
//#include "NoaMath.h"
#include "Transform.h"

using namespace std;

namespace noa {
	//游戏脚本类

	class Transform;

	class Actor {
	private:
		bool isActive = true;
	public:
		Transform transform;

	public:
		Actor();
		~Actor();
	public:
		void Awake();
		virtual void OnEnable() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void OnDisable() {}
		virtual void Destroy();
		void SetActive(bool value);
		bool GetActive();

		// 获取哈希值
		size_t GetHash() const {
			return id;
		}

		// 将Behaviour作为其他组件返回
		template<class T>
		T GetComponentAs() {
			return (T)this;
		}

		Actor* GetActor() {
			return this;
		}

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
