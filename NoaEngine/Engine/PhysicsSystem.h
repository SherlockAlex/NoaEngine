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

	class NOA_API Rigidbody;
	class NOA_API Collider2D;
	class NOA_API CircleCollider2D;
	class NOA_API BoxCollider2D;

	enum class NOA_API ColliderType; // 前置声明

	class NOA_API PhysicsSystem
	{
	public:
		static void InitPhysicsSystem();
		static void Update(int step);
	private:
		static void FindCollisions();
		static bool CircleCollide(
			Collider2D* obj1
			, Collider2D* obj2
			, noa::Vector<float>* normal
			, float* depth
		);
		static bool PolygonsCollide(
			Collider2D* obj1
			, Collider2D* obj2
			, noa::Vector<float>* normal
			, float* depth
		);
		static bool PolygonAndCircleCollide(
			Collider2D* obj1
			, Collider2D* obj2
			, noa::Vector<float>* normal
			, float* depth
		);
		static bool Collide(
			Collider2D* obj1
			, Collider2D* obj2
			, noa::Vector<float>* normal
			, float* depth
		);
		static void SolveCollision(
			Collider2D* obj1
			, Collider2D* obj2
			, noa::Vector<float> normal
			, float depth
		);

		//刚体求解相关
		static void InitVelocity(float deltaTime);
		static void ApplyVelocity(float deltaTime);
		static void InitPosition(float deltaTime);
		static void ApplyTileCollision(float deltaTime);
		static void ApplyTileConstraint(float deltaTime);
		static void ApplyPosition(float deltaTime);

	public:
		//物理引擎参数

		static int step;
		static noa::Vector<float> gravity;
	private:
		static std::vector<Collider2D*> colliders;
		static std::vector<Rigidbody*> rigidbodys;

		static std::unordered_map<ColliderType, std::unordered_map<noa::ColliderType, std::function<bool(
			Collider2D*
			, Collider2D*
			, Vector<float>*
			, float*)>>> collideFunc;

		friend class Collider2D;
		friend class Rigidbody;


	};

}

#endif // !NOAENGINE_PHYSICSSYSTEM_H


