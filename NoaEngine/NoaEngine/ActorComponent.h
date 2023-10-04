#ifndef NOAENGINE_ACTORCOMPONENT_H
#define NOAENGINE_ACTORCOMPONENT_H

namespace noa {
	class Actor;

	/// <summary>
	/// 组件类，只有ActorComponent及其子类才能够成为组件在挂在到Actor身上
	/// </summary>
	class ActorComponent
	{
	protected:
		Actor* actor = nullptr;
	private:
		bool active = true;

	protected:
		ActorComponent(Actor* actor);
		virtual ~ActorComponent();

	public:
		static ActorComponent* Create(Actor* actor);
		virtual void Delete();

		virtual void Awake() {};
		virtual void OnEnable() {}
		virtual void Start() {}
		virtual void Update() {}
		virtual void OnDisable() {}
		virtual void OnDestroy() {}

		virtual void SetActive(bool value);
		virtual bool GetActive();

		void DeleteActorEvent();

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



