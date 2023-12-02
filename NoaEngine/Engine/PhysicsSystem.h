#ifndef NOAENGINE_PHYSICSSYSTEM_H
#define NOAENGINE_PHYSICSSYSTEM_H

#include <vector>
#include "NoaMath.h"
#include "Core.h"

/**
* 物理系统
* 一个物理系统应该是若干个刚体质点的集合
* 一个大的物理系统包含若干个子物理系统
* 
* 对于游戏世界而言，对速度和位置进行约束往往更好，可以使用分析求解哈密顿方程组
* 游戏的物理，除了要和物体进行交互，还要和声音模块进行一个耦合
* 
*/



namespace noa 
{
	struct NOA_API Grid;
	struct NOA_API Cell;

	class NOA_API Rigidbody;
	class NOA_API Collider2D;
	class NOA_API CircleCollider2D;
	class NOA_API BoxCollider2D;

	// 定义AABB结构体
	struct NOA_API AABB {
		float minX, maxX;
		float minY, maxY;
	};

	class NOA_API PhysicsSystem
	{
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

	private:
		static void InitVelocity(float deltaTime);
		static void ApplyVelocity(float deltaTime);
		static void InitPosition(float deltaTime);
		static void ApplyTileCollision(float deltaTime);
		static void ApplyTileConstraint(float deltaTime);
		static void ApplyPosition(float deltaTime);

	public:
		static int step;
		static Grid grid;
		static std::vector<Rigidbody*> rigidbodys;
		static noa::Vector<float> gravity;
	};

}

#endif // !NOAENGINE_PHYSICSSYSTEM_H


