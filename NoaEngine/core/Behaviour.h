#ifndef NOAENGINE_BEHAVIOUR_H
#define NOAENGINE_BEHAVIOUR_H

#include <vector>
//#include "NoaMath.h"
#include "Transform.h"

using namespace std;

namespace noa {
	//��Ϸ�ű���

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

		// ��ȡ��ϣֵ
		size_t GetHash() const {
			return id;
		}

	private:
		size_t id;
		static size_t nextId;

		// ��̬���������ڻ�ȡ��һ����һ�޶��Ĺ�ϣֵ
		static size_t GetNextId() {
			return nextId++;
		}

	};

	//������Ϸ�ű�
	//extern void Destroy(Behaviour* behaviour);
}



#endif
