#ifndef NOAENGINE_ACTOR_H
#define NOAENGINE_ACTOR_H

#include <vector>
//#include "NoaMath.h"
#include "Transform.h"

using namespace std;

namespace noa {
	//��Ϸ�ű���

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

		// ��ȡ��ϣֵ
		size_t GetHash() const {
			return id;
		}

		// ��Behaviour��Ϊ�����������
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

		// ��̬���������ڻ�ȡ��һ����һ�޶��Ĺ�ϣֵ
		static size_t GetNextId() {
			return nextId++;
		}

	};

}



#endif
