#ifndef NOAENGINE_PHYSICSSYSTEM_H
#define NOAENGINE_PHYSICSSYSTEM_H

#include <vector>
#include "NoaMath.h"

namespace noa 
{
	struct Grid;
	struct Cell;

	class Rigidbody;
	class Collider2D;
	class CircleCollider2D;
	class BoxCollider2D;

	// ����AABB�ṹ��
	struct AABB {
		float minX, maxX;
		float minY, maxY;
	};

	class PhysicsSystem
	{
	public:
		static Grid grid;
		static std::vector<Rigidbody*> rigidbodys;
		static Vector<float> gravity;
	public:
		static void SetGrid(int width,int height);
		static void Update(int step);

	public:

		static void FindCollisionsGrid();

		static void CheckCellsCollisions(Cell& cell1, Cell& cell2);

		static bool CircleCollide(CircleCollider2D * obj1,CircleCollider2D * obj2);

		static bool BoxCollide(BoxCollider2D * obj1,BoxCollider2D * obj2);

		static bool BoxAndCircleCollide(BoxCollider2D* obj1, CircleCollider2D* obj2);

		static bool Collide(Collider2D* obj1, Collider2D* obj2);

		static bool Collide(const AABB& box1, const AABB& box2);

		static void SolveCollision(Collider2D* obj1, Collider2D* obj2);
	};
}

#endif // !NOAENGINE_PHYSICSSYSTEM_H


