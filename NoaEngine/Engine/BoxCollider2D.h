#ifndef NOAENGINE_BOXCOLLIDER2D_H
#define NOAENGINE_BOXCOLLIDER2D_H

#include "Collider2D.h"
#include "Core.h"

namespace noa {
	class NOA_API BoxCollider2D final :public Collider2D {
	private:
		ACTOR_COMPONENT(BoxCollider2D)
	private:
		BoxCollider2D(Actor* actor);
		~BoxCollider2D() override;

	public:
		static BoxCollider2D* Create(Actor* actor);
		BoxCollider2D& SetScale(float x, float y);
		BoxCollider2D& SetRigidbody(Rigidbody* rigidbody);
		BoxCollider2D& SetTrigger(bool isTrigger);
		BoxCollider2D* Apply();
	public:
		Vector<float> scale = { 1,1 };
	};
}

#endif // !NOAENGINE_BOXCOLLIDER2D_H

