#ifndef NOAENGINE_ACTOR_H
#define NOAENGINE_ACTOR_H

#include <vector>
#include "Transform.h"

namespace noa {
	//��Ϸ�ű���

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

		// ��ȡ��ϣֵ
		size_t GetHash() const {
			return id;
		}

		// ��Behaviour��Ϊ�����������
		template<class T>
		T GetActorAs() {
			return dynamic_cast<T>(this);
		}

		Actor* GetActor() {
			return this;
		}

		void SetActiveScene(Scene * scene);

		//���������������ڴ�������other��һ��void*���͵�ָ��
		virtual void OnTrigger(Collision collision);

		virtual void OnHitTile() {}

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
