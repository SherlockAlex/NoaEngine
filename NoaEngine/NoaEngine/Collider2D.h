#ifndef NOAENGINE_COLLIDER2D_H
#define NOAENGINE_COLLIDER2D_H

#include "ActorComponent.h"
#include <vector>

namespace noa 
{

	enum class ColliderType {
		CIRCLE_COLLIDER,
		BOX_COLLIDER
	};

	class Rigidbody;
	class Collider2D:public ActorComponent
	{
	protected:
		friend class PhysicsSystem;
		ColliderType colliderType;
	public:
		Rigidbody* rigidbody = nullptr;

	protected:
		Collider2D(Actor* actor, Rigidbody* rigidbody);
		virtual ~Collider2D() override;

	public:
		static Collider2D* Create(Actor * actor,Rigidbody * rigidbody);
		virtual void Update() override;
		virtual void Delete() override;

	};

	class BoxCollider2D :public Collider2D {
	public:
		Vector<float> scale = {1,1};
	private:
		BoxCollider2D(Actor * actor,Rigidbody* rigidbody);
		~BoxCollider2D() override;

	public:
		static BoxCollider2D* Create(Actor * actor,Rigidbody * rigidbody);

	};

	class CircleCollider2D :public Collider2D
	{
	public:
		float radius = 0.5f;
	private:
		CircleCollider2D(Actor* actor, Rigidbody* rigidbody);
		~CircleCollider2D() override;
	public:
		static CircleCollider2D* Create(Actor* actor, Rigidbody* rigidbody);

	};

	typedef struct Cell {
		std::vector<Collider2D*> colliders;
	}Cell;

	typedef struct Grid
	{
		int width;
		int height;

		std::vector<Cell> cells;

		//返回坐标为x,y的cell
		Cell * GetCell(int x, int y);

	}Grid;

}

#endif // !NOAENGINE_COLLIDER2D_H