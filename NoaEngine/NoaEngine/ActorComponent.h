#ifndef NOAENGINE_ACTORCOMPONENT_H
#define NOAENGINE_ACTORCOMPONENT_H

#include "NObject.h"

namespace noa {
	class Actor;

	/// <summary>
	/// ����ֻ࣬��ActorComponent����������ܹ���Ϊ����ڹ��ڵ�Actor����
	/// </summary>
	class ActorComponent:public NOAObject
	{
	protected:
		friend class NObject<ActorComponent>;
		friend void InitNObject(NOAObject* obj);
		friend class Actor;

		Actor* actor = nullptr;
	private:
		bool active = true;

	protected:
		ActorComponent(Actor* actor);
		virtual ~ActorComponent();

	protected:
		void Delete();

		virtual void Awake() {};
		virtual void OnEnable() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void OnDisable() {}
		virtual void OnDestroy() {}

		void DeleteActorEvent();

	public:

		virtual void SetActive(bool value);
		virtual bool GetActive();

		

		template<class T>
		T GetActorAs() {
			return dynamic_cast<T>(actor);
		}

		template<class T>
		bool TryGetActorAs(T & out) 
		{
			T buffer = dynamic_cast<T>(actor);
			if (buffer == nullptr)
			{
				out = nullptr;
				return false;
			}
			out = buffer;
			return true;
		}

		template<class T>
		T GetComponentAs() {
			return dynamic_cast<T>(this);
		}

		template<class T>
		bool TryGetComponentAs(T & out) 
		{
			T buffer = dynamic_cast<T>(this);
			if (buffer==nullptr)
			{
				out = nullptr;
				return false;
			}
			out = buffer;
			return true;
		}


	};
}



#endif // !NOAENGINE_ACTORCOMPONENT_H



