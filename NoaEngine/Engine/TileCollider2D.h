#ifndef NOAENGINE_TILECOLLIDER2D_H
#define NOAENGINE_TILECOLLIDER2D_H

#include "Collider2D.h"
#include "Core.h"


namespace noa {

	//ֻ���tile���н����������������collider���н���
	//��������ճ�������bug
	//���������ͳһ���е�Collider

	class NOA_API TileCollider2D final :public Collider2D
	{
	private:
		ACTOR_COMPONENT(TileCollider2D)
			friend class Rigidbody;
	private:
		TileCollider2D(Actor* actor);
		~TileCollider2D();

		bool CheckCollision(float x, float y);
		bool CheckCollision(int x1, int y1, int x2, int y2);

	public:
		static TileCollider2D* Create(Actor* actor);
		void Update() override;
		void LateUpdate() override;

		void ApplyTileCollision(float deltaTime);
		void ApplyConstraint(float deltaTime);

		TileCollider2D& SetTileMap(TileMap* tileMap);
		TileMap* GetTileMap();
		TileCollider2D& SetSize(float x, float y);
		TileCollider2D& SetRigidbody(Rigidbody* rigidbody);
		TileCollider2D& SetTrigger(bool value);
		TileCollider2D* Apply();

	private:
		TileMap* tileMap = nullptr;
		bool isHitCollisionTile = false;
	public:

		Vector<float> size = { 1,1 };
		bool isGrounded = false;
	private:

	};

}

#endif // !NOAENGINE_TILECOLLIDER2D_H
