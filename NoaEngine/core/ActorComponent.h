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

	};
}



#endif // !NOAENGINE_ACTORCOMPONENT_H



