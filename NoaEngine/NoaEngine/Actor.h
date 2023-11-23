#ifndef NOAENGINE_ACTOR_H
#define NOAENGINE_ACTOR_H

#include <vector>
#include <functional>

#include "Transform.h"
#include "NObject.h"
#include "Scene.h"

/*
* Actorģ��
* ����:��Ϸ����Scene�еĻ�Ķ��󣬿ɹ������ʵ�ָ��ָ�������Ϸ��Ϊ
* ����Unity�е�GameObject
* 
* Actorʱ��Ϸ�Ļ�����Ԫ��Camera����Actor
* 
* ͬʱActor���Ը��ݲ�ͬ��ActorComponentʵ�ֲ�ͬ�Ĺ���
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
			//���Ի�ȡT���͵����
			//���û�л�ȡ������false
			//���򷵻�True����ִ�лص�����
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
