#ifndef NOAENGINE_COLLIDER2D_H
#define NOAENGINE_COLLIDER2D_H

#include "ActorComponent.h"
#include <vector>

namespace noa 
{
	class Rigidbody;
	class Collider2D:public ActorComponent
	{
	public:
		Rigidbody* rigidbody = nullptr;

	protected:
		Collider2D(Actor* actor, Rigidbody* rigidbody);
		virtual ~Collider2D();

	public:
		static Collider2D* Create(Actor * actor,Rigidbody * rigidbody);
		virtual void Update() override;
		virtual void Delete() override;

	};

	class CircleCollider2D :public Collider2D
	{
	public:
		float radius = 1;

	private:
		CircleCollider2D(Actor* actor, Rigidbody* rigidbody);
		~CircleCollider2D();
	public:
		static CircleCollider2D* Create(Actor* actor, Rigidbody* rigidbody);
		void Delete() override;

	};

	typedef struct Cell {
		std::vector<CircleCollider2D*> colliders;
	}Cell;

	typedef struct Grid
	{
		int width;
		int height;

		std::vector<Cell> cells;

		//返回坐标为x,y的cell
		Cell& GetCell(int x, int y);

	}Grid;

}

#endif // !NOAENGINE_COLLIDER2D_H