#ifndef NOAENGINE_CIRCLECOLLIDER2D_H
#define NOAENGINE_CIRCLECOLLIDER2D_H

#include "Collider2D.h"

namespace noa {
	class CircleCollider2D final :public Collider2D
	{
	private:
		ACTOR_COMPONENT(CircleCollider2D)
	public:
		float radius = 0.5f;
	private:
		CircleCollider2D(Actor* actor);
		~CircleCollider2D() override;
	public:
		static CircleCollider2D* Create(Actor* actor);
		CircleCollider2D& SetRigidbody(Rigidbody* rigidbody);
		CircleCollider2D& SetRadius(float radius);
		CircleCollider2D& SetIsTrigger(bool isTrigger);
		CircleCollider2D* Apply();

	};

}

#endif // !NOAENGINE_CIRCLECOLLIDER2D_H


