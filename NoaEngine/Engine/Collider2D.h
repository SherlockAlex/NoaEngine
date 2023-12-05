#ifndef NOAENGINE_COLLIDER2D_H
#define NOAENGINE_COLLIDER2D_H

#include "ActorComponent.h"
#include "NoaMath.h"
#include "Core.h"
#include <vector>

namespace noa 
{
	class NOA_API TileMap;
	enum class NOA_API ColliderType {
		TILE_COLLIDER,
		CIRCLE_COLLIDER,
		BOX_COLLIDER
	};

	// 只要绑定了Collider2D，就必须要绑定一个rigidbody
	class NOA_API Rigidbody;
	class NOA_API Collider2D :public ActorComponent
	{
	protected:
		Collider2D(Actor* actor);
		virtual ~Collider2D() override;
		void ApplyTriggerEnter(Collider2D& other);
		virtual void Update() override;
		void SetRigidbody(Rigidbody* rigidbody);
		void SetTrigger(bool value);
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

	//typedef NOA_API struct Cell {
	//	std::vector<Collider2D*> colliders;
	//}Cell;

	//typedef struct NOA_API Grid
	//{
	//	int width = 0;
	//	int height = 0;

	//	std::vector<Cell> cells;

	//	//返回坐标为x,y的cell
	//	Cell * GetCell(int x, int y);

	//}Grid;

}

#endif // !NOAENGINE_COLLIDER2D_H