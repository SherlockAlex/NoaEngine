#ifndef NOAENGINE_PHYSICSSYSTEM_H
#define NOAENGINE_PHYSICSSYSTEM_H

#include <vector>
#include "NoaMath.h"
#include "Core.h"

/**
* ����ϵͳ
* һ������ϵͳӦ�������ɸ������ʵ�ļ���
* һ���������ϵͳ�������ɸ�������ϵͳ
* 
* ������Ϸ������ԣ����ٶȺ�λ�ý���Լ���������ã�����ʹ�÷��������ܶٷ�����
* ��Ϸ����������Ҫ��������н�������Ҫ������ģ�����һ�����
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

	// ����AABB�ṹ��
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


