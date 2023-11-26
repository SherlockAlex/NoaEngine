#ifndef NOAENGINE_CIRCLECOLLIDER2D_H
#define NOAENGINE_CIRCLECOLLIDER2D_H

#include "Collider2D.h"

namespace noa {
	class CircleCollider2D final :public Collider2D
	{
	private:
		ACTOR_COMPONENT(CircleCollider2D)
	private:
		CircleCollider2D(Actor* actor);
		~CircleCollider2D() override;
	public:
		static CircleCollider2D* Create(Actor* actor);
		CircleCollider2D& SetRigidbody(Rigidbody* rigidbody);
		CircleCollider2D& SetRadius(float radius);
		CircleCollider2D& SetTrigger(bool isTrigger);
		CircleCollider2D& SetOffset(float x,float y);
		CircleCollider2D* Apply();
	public:
		float radius = 0.5f;

	};

}

#endif // !NOAENGINE_CIRCLECOLLIDER2D_H


