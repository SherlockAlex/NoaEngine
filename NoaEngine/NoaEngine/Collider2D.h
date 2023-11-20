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
	class Collider2D :public ActorComponent
	{
	
	protected:
		Collider2D(Actor* actor);
		virtual ~Collider2D() override;
		void ApplyTriggerEnter(Collider2D& other);
		virtual void Update() override;
		void SetRigidbody(Rigidbody* rigidbody);
	public:

		template<class T>
		T* GetCollider2DAs() {
			return dynamic_cast<T*>(this);
		}

	protected:
		friend class PhysicsSystem;
		friend class Rigidbody;

		Rigidbody* rigidbody = nullptr;
		ColliderType colliderType;
	public:
		bool isTrigger = false;
		Vector<float> offset;
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