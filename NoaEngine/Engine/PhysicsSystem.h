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

	class NOA_API Rigidbody;
	class NOA_API Collider2D;
	class NOA_API CircleCollider2D;
	class NOA_API BoxCollider2D;

	class NOA_API PhysicsSystem
	{
	public:
		static void Update(int step);
	private:
		static void FindCollisions();
		static bool CircleCollide(
			CircleCollider2D* obj1
			, CircleCollider2D* obj2
			, noa::Vector<float>* normal
			,float* depth
		);
		static bool PolygonsCollide(
			Collider2D * obj1
			, Collider2D* obj2
			, noa::Vector<float>* normal
			, float* depth
		);
		static bool BoxAndCircleCollide(BoxCollider2D* obj1, CircleCollider2D* obj2);
		static bool Collide(
			Collider2D* obj1
			, Collider2D* obj2
			, noa::Vector<float>* normal
			, float* depth
		);
		static void SolveCollision(
			Collider2D* obj1
			, Collider2D* obj2
			,noa::Vector<float> normal
			,float depth
		);

		//����������
		static void InitVelocity(float deltaTime);
		static void ApplyVelocity(float deltaTime);
		static void InitPosition(float deltaTime);
		static void ApplyTileCollision(float deltaTime);
		static void ApplyTileConstraint(float deltaTime);
		static void ApplyPosition(float deltaTime);

	public:
		//�����������

		static int step;
		static noa::Vector<float> gravity;
	private:
		static std::vector<Collider2D*> colliders;
		static std::vector<Rigidbody*> rigidbodys;

		friend class Collider2D;
		friend class Rigidbody;

	};

}

#endif // !NOAENGINE_PHYSICSSYSTEM_H


