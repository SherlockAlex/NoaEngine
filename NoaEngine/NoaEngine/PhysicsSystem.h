#ifndef NOAENGINE_PHYSICSSYSTEM_H
#define NOAENGINE_PHYSICSSYSTEM_H

#include <vector>
#include "NoaMath.h"

/**
* 物理系统
* 一个物理系统应该是若干个刚体质点的集合
* 一个大的物理系统包含若干个子物理系统
* 
* 对于游戏世界而言，一切改变都应该服从牛顿定律，及一般不去改变速度，而是去修正力
* 
*/



namespace noa 
{
	struct Grid;
	struct Cell;

	class Rigidbody;
	class Collider2D;
	class CircleCollider2D;
	class BoxCollider2D;

	// 定义AABB结构体
	struct AABB {
		float minX, maxX;
		float minY, maxY;
	};

	class PhysicsSystem
	{
	public:
		static int step;

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

	private:
		static void InitVelocity(float deltaTime);
		static void ApplyVelocity(float deltaTime);
		static void InitPosition(float deltaTime);
		static void ApplyTileCollision(float deltaTime);
		static void ApplyTileConstraint(float deltaTime);
		static void ApplyPosition(float deltaTime);
	};

}

#endif // !NOAENGINE_PHYSICSSYSTEM_H


