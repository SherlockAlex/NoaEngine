#ifndef NOAENGINE_BEHAVIOUR_H
#define NOAENGINE_BEHAVIOUR_H

#include <vector>
//#include "NoaMath.h"
#include "Transform.h"

using namespace std;

namespace noa {
	//游戏脚本类

	class Transform;

	class Behaviour {
	private:
		bool isActive = true;
	public:
		Transform transform;
		//int hashCode = 0;

	public:
		Behaviour();
		~Behaviour();
	public:
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

	private:
		size_t id;
		static size_t nextId;

		// 静态函数，用于获取下一个独一无二的哈希值
		static size_t GetNextId() {
			return nextId++;
		}

	};

	//销毁游戏脚本
	//extern void Destroy(Behaviour* behaviour);
}



#endif
