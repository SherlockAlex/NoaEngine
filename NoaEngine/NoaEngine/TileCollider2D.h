#ifndef NOAENGINE_TILECOLLIDER2D_H
#define NOAENGINE_TILECOLLIDER2D_H

#include "Collider2D.h"

namespace noa {

	class TileCollider2D final :public Collider2D
	{
	private:
		ACTOR_COMPONENT(TileCollider2D)
			friend class Rigidbody;
	private:
		TileCollider2D(Actor* actor);
		~TileCollider2D();
	public:
		static TileCollider2D* Create(Actor* actor);
		void Update() override;
		void LateUpdate() override;

		void ApplyTileCollision(float deltaTime);
		void ApplyConstraint(float deltaTime);

		TileCollider2D& SetTileMap(TileMap* tileMap);
		TileCollider2D& SetScale(float x, float y);
		TileCollider2D& SetRigidbody(Rigidbody* rigidbody);
		TileCollider2D& SetIsTrigger(bool value);
		TileCollider2D* Apply();

	private:
		TileMap* tileMap = nullptr;
		bool isHitCollisionTile = false;
	public:

		Vector<float> scale = { 1,1 };
		bool isGrounded = false;
	private:

	};

}

#endif // !NOAENGINE_TILECOLLIDER2D_H
