#ifndef NOAENGINE_PHYSICSSYSTEM_H
#define NOAENGINE_PHYSICSSYSTEM_H


#include "Collider2D.h"

namespace noa 
{

	class PhysicsSystem
	{
	public:
		static Grid grid;
		static std::vector<Rigidbody*> rigidbodys;
	public:
		static void SetGrid(int width,int height);
		static void Update(int step);

	private:

		static void FindCollisionsGrid();

		static void CheckCellsCollisions(Cell& cell1, Cell& cell2);

		static bool CircleCollide(CircleCollider2D * obj1,CircleCollider2D * obj2);

		static bool BoxCollide(BoxCollider2D * obj1,BoxCollider2D * obj2);

		static bool BoxAndCircleCollide(BoxCollider2D* obj1, CircleCollider2D* obj2);

		static bool Collide(Collider2D* obj1, Collider2D* obj2);

		static void SolveCollision(Collider2D* obj1, Collider2D* obj2);
	};
}

#endif // !NOAENGINE_PHYSICSSYSTEM_H


