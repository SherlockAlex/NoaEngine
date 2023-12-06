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
		POLYGON_COLLIDER
	};

	// ֻҪ����Collider2D���ͱ���Ҫ��һ��rigidbody
	class NOA_API Rigidbody;
	class NOA_API Collider2D :public ActorComponent
	{
	protected:
		Collider2D(Actor* actor);
		virtual ~Collider2D() override;
		void ApplyTriggerEnter(Collider2D& other);
		virtual void Update() override;
		virtual void UpdateCaculateVertices();
		void SetRigidbody(Rigidbody* rigidbody);
		void SetTrigger(bool value);
		
	public:
		noa::Rigidbody* GetRigidbody();
		template<class T>
		T* GetCollider2DAs() {
			return dynamic_cast<T*>(this);
		}

	protected:
		friend class PhysicsSystem;
		friend class Rigidbody;

		Rigidbody* rigidbody = nullptr;
		ColliderType colliderType;

		std::vector<noa::Vector<float>> vertices;
		std::vector<noa::Vector<float>> caculateVertices;

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

	//	//��������Ϊx,y��cell
	//	Cell * GetCell(int x, int y);

	//}Grid;

}

#endif // !NOAENGINE_COLLIDER2D_H