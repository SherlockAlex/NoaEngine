#ifndef NOAENGINE_ACTORMOTION_H
#define NOAENGINE_ACTORMOTION_H

#include <vector>
#include <functional>

#include "ActorComponent.h"

namespace noa {

	typedef struct Motion {
		Vector<float> destination;
		float speed = 0.0f;
		NoaEvent<void> finishedEvent;
	}Motion;

	class Rigidbody;
	class ActorMotion final:
		public ActorComponent
	{
	private:
		ACTOR_COMPONENT(ActorMotion)
	private:
		ActorMotion(Actor* actor);
		~ActorMotion();

	public:
		static ActorMotion* Create(Actor* actor);

		ActorMotion& SetRigidbody(Rigidbody* rigidbody);
		ActorMotion& AddMotion(const Vector<float>& destination
			,float speed);
		ActorMotion& SetCallback(size_t index,std::function<void()> action);
		ActorMotion& SetPrecision(float precision);
		ActorMotion& SetLoop(bool value);
		ActorMotion& Act();
		ActorMotion& Stop();
		ActorMotion* Apply();

	private:
		void Start() override;
		void Update() override;

	private:

		size_t currentMotionIndex = 0;
		Actor* actorHandle = nullptr;
		Rigidbody* rigidbodyHandle = nullptr;
		Vector<float> velocity;

		//下一个position位置，移动速度
		std::vector<Motion> motions;
		size_t motionCount = 0;

		float precision = 0.1f;
		bool loop = false;

		bool finished = true;

	};
}

#endif // !NOAENGINE_ACTORMOTION_H



