#ifndef NOAENGINE_ACTORCOMPONENT_H
#define NOAENGINE_ACTORCOMPONENT_H

namespace noa {
	class Actor;

	/// <summary>
	/// ����ֻ࣬��ActorComponent����������ܹ���Ϊ����ڹ��ڵ�Actor����
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



