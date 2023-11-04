#ifndef NOAENGINE_COLLIDER2D_H
#define NOAENGINE_COLLIDER2D_H

#include "ActorComponent.h"
#include "NoaMath.h"
#include <vector>

namespace noa 
{
	class TileMap;
	enum class ColliderType {
		TILE_COLLIDER,
		CIRCLE_COLLIDER,
		BOX_COLLIDER
	};

	class Rigidbody;
	class Collider2D:public ActorComponent
	{
	protected:
		friend class PhysicsSystem;
		friend class Rigidbody;
		ColliderType colliderType;
	public:
		Rigidbody* rigidbody = nullptr;
		bool isTrigger = false;

	protected:
		Collider2D(Actor* actor);
		virtual ~Collider2D() override;
		void ApplyTrigger();
	public:
		virtual void Update() override;

		template<class T>
		T* GetCollider2DAs() {
			return dynamic_cast<T*>(this);
		}

	protected:
		void SetRigidbody(Rigidbody * rigidbody);
		
	};

	class BoxCollider2D final:public Collider2D {
	private:
		NOBJECT(BoxCollider2D)
	public:
		Vector<float> scale = {1,1};
	private:
		BoxCollider2D(Actor * actor);
		~BoxCollider2D() override;

	public:
		static BoxCollider2D* Create(Actor * actor);
		BoxCollider2D* SetScale(int x, int y);
		BoxCollider2D* SetRigidbody(Rigidbody * rigidbody);
		BoxCollider2D* SetIsTrigger(bool isTrigger);
		BoxCollider2D* Apply();
	};

	class CircleCollider2D final:public Collider2D
	{
	private:
		NOBJECT(CircleCollider2D)
	public:
		float radius = 0.5f;
	private:
		CircleCollider2D(Actor* actor);
		~CircleCollider2D() override;
	public:
		static CircleCollider2D* Create(Actor* actor);
		CircleCollider2D * SetRigidbody(Rigidbody* rigidbody);
		CircleCollider2D * SetRadius(float radius);
		CircleCollider2D * SetIsTrigger(bool isTrigger);
		CircleCollider2D * Apply();

	};

	class TileCollider2D final:public Collider2D 
	{
	private:
		NOBJECT(TileCollider2D)
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

		TileCollider2D* SetTileMap(TileMap* tileMap);
		TileCollider2D* SetScale(float x,float y);
		TileCollider2D* SetRigidbody(Rigidbody * rigidbody);
		TileCollider2D* Apply();

	private:
		TileMap* tileMap = nullptr;
		bool isHitCollisionTile = false;
	public:
		
		Vector<float> scale = {1,1};
		bool isGrounded = false;
	private:
		

	};

	typedef struct Cell {
		std::vector<Collider2D*> colliders;
	}Cell;

	typedef struct Grid
	{
		int width = 0;
		int height = 0;

		std::vector<Cell> cells;

		//返回坐标为x,y的cell
		Cell * GetCell(int x, int y);

	}Grid;

}

#endif // !NOAENGINE_COLLIDER2D_H