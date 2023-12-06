#include "Collider2D.h"
#include "BoxCollider2D.h"
#include "CircleCollider2D.h"
#include "TileCollider2D.h"

#include "PhysicsSystem.h"
#include "Time.h"
#include "Rigidbody.h"

std::vector<noa::Rigidbody*> noa::PhysicsSystem::rigidbodys;
std::vector<noa::Collider2D*> noa::PhysicsSystem::colliders;

noa::Vector<float> noa::PhysicsSystem::gravity = { 0,9.82f };

int noa::PhysicsSystem::step = 5;

std::unordered_map<noa::ColliderType, std::unordered_map<noa::ColliderType, std::function<bool(
	noa::Collider2D*
	, noa::Collider2D*
	, noa::Vector<float>*
	, float*)>>> noa::PhysicsSystem::collideFunc;

void noa::PhysicsSystem::InitPhysicsSystem() {
	collideFunc[ColliderType::CIRCLE_COLLIDER][ColliderType::CIRCLE_COLLIDER] = CircleCollide;
	collideFunc[ColliderType::POLYGON_COLLIDER][ColliderType::CIRCLE_COLLIDER] = PolygonAndCircleCollide;
	collideFunc[ColliderType::CIRCLE_COLLIDER][ColliderType::POLYGON_COLLIDER] = PolygonAndCircleCollide;
	collideFunc[ColliderType::POLYGON_COLLIDER][ColliderType::POLYGON_COLLIDER] = PolygonsCollide;
}

void noa::PhysicsSystem::Update(int step)
{

	const float subDeltaTime = Time::deltaTime / static_cast<float>(step);
#pragma omp parallel for
	for (int i{step};i--;)
	{

		InitVelocity(subDeltaTime);			
		ApplyVelocity(subDeltaTime);		
		InitPosition(subDeltaTime);			
		ApplyTileCollision(subDeltaTime);	
		ApplyTileConstraint(subDeltaTime);	
		FindCollisions();
		ApplyTileCollision(subDeltaTime);	
		ApplyTileConstraint(subDeltaTime);	
		ApplyPosition(subDeltaTime);		
		
	}
	
	colliders.clear();
	rigidbodys.clear();
}

void noa::PhysicsSystem::FindCollisions() 
{
	const int colliderCount = static_cast<int>(colliders.size());
	for (int i = 0;i< colliderCount-1;i++)
	{
		for (int j = i+1;j< colliderCount;j++)
		{
			Collider2D* collider1 = colliders[i];
			Collider2D* collider2 = colliders[j];

			collider1->UpdateCaculateVertices();
			collider2->UpdateCaculateVertices();

			float depth = 0.0f;
			noa::Vector<float> normal = {};

			if (Collide(collider1, collider2,&normal,&depth))
			{
				const bool isAllTrigger = 
					collider1->isTrigger
					&& collider2->isTrigger;
				if (isAllTrigger)
				{
					// 如果说两个都是Trigger，那么不进行碰撞处理
					continue;
				}

				if (!collider1->isTrigger && !collider2->isTrigger)
				{
					// 如果说两个都不是isTrigger，那么两个进行一个碰撞处理
					SolveCollision(
						collider1
						, collider2
						,normal
						,depth
					);
				}

				//两者其中一个是trigger

				collider1->ApplyTriggerEnter(*collider2);
				collider2->ApplyTriggerEnter(*collider1);

			}

		}
	}
}

bool noa::PhysicsSystem::Collide(
	Collider2D* obj1
	, Collider2D* obj2
	, noa::Vector<float>* normal
	, float* depth
)
{

	//碰撞检测
	//下面代码可以优化，通过

	if (collideFunc.count(obj1->colliderType)<=0
		|| collideFunc[obj1->colliderType].count(obj2->colliderType)<=0)
	{
		return false;
	}

	return collideFunc[obj1->colliderType][obj2->colliderType]
	(
		obj1
		, obj2
		, normal
		, depth);
	
}

void noa::PhysicsSystem::SolveCollision(
	Collider2D* obj1
	, Collider2D* obj2
	, noa::Vector<float> normal
	, float depth
)
{
	//求解碰撞

	Rigidbody* rigid1 = obj1->rigidbody;
	Rigidbody* rigid2 = obj2->rigidbody;

	const float fixX = depth * normal.x;
	const float fixY = depth * normal.y;

	rigid1->newPosition.x += 0.5f * fixX;
	rigid1->newPosition.y += 0.5f * fixY;

	rigid2->newPosition.x -= 0.5f * fixX;
	rigid2->newPosition.y -= 0.5f * fixY;

	//计算接触力
	/*const float sumInvInvMass = 1.0f/(rigid1->invMass + rigid2->invMass);

	noa::Vector<float> relativeVelocity =
		rigid2->velocity - rigid1->velocity;
	const float restitude = 0.8f;
	const float beta =
		-(1.0f-restitude)
		*noa::Vector<float>::Dot(normal,relativeVelocity)
		* sumInvInvMass;*/

	const float beta = -125.0f;

	const Vector<float> constraintImpulse1 = {
		 -beta * fixX
		,-beta * fixY
	};
	const Vector<float> constraintImpulse2 = {
		beta * fixX
		,beta * fixY
	};

	rigid1->AddForce(
		constraintImpulse1, noa::ForceType::IMPULSE_FORCE);
	rigid2->AddForce(
		constraintImpulse2, noa::ForceType::IMPULSE_FORCE);

}

void noa::PhysicsSystem::InitVelocity(float deltaTime)
{
#pragma omp parallel for
	for (auto& rigidbody : rigidbodys)
	{
		rigidbody->InitVelocity(deltaTime);
	}
}

void noa::PhysicsSystem::ApplyVelocity(float deltaTime)
{
#pragma omp parallel for
	for (auto& rigidbody : rigidbodys)
	{
		rigidbody->ApplyVelocity(deltaTime);
	}
}

void noa::PhysicsSystem::InitPosition(float deltaTime)
{
#pragma omp parallel for
	for (auto& rigidbody : rigidbodys)
	{
		rigidbody->InitPosition(deltaTime);
	}
}

void noa::PhysicsSystem::ApplyTileCollision(float deltaTime)
{
#pragma omp parallel for
	for (auto& rigidbody : rigidbodys)
	{
		rigidbody->ApplyTileCollision(deltaTime);
	}
}

void noa::PhysicsSystem::ApplyTileConstraint(float deltaTime)
{
#pragma omp parallel for
	for (auto& rigidbody : rigidbodys)
	{
		rigidbody->ApplyTileConstraint(deltaTime);
	}
}


void noa::PhysicsSystem::ApplyPosition(float deltaTime)
{
#pragma omp parallel for
	for (auto& rigidbody : rigidbodys)
	{
		rigidbody->ApplyPositon(deltaTime);
	}
}

bool noa::PhysicsSystem::CircleCollide(
	Collider2D* collider1
	, Collider2D* collider2
	, noa::Vector<float>* normalPtr
	, float* depthPtr)
{

	CircleCollider2D* obj1 = dynamic_cast<CircleCollider2D*>(collider1);
	CircleCollider2D* obj2 = dynamic_cast<CircleCollider2D*>(collider2);

	if (!obj1 || !obj2)
	{
		return false;
	}

	noa::Vector<float> normal = {};
	float depth = 0.0f;

	Rigidbody* rigid1 = obj1->rigidbody;
	Rigidbody* rigid2 = obj2->rigidbody;

	const float deltaX = rigid1->newPosition.x + obj1->offset.x
		- rigid2->newPosition.x - obj2->offset.x;

	const float deltaY = rigid1->newPosition.y + obj1->offset.y
		- rigid2->newPosition.y - obj2->offset.y;

	const float deltaR = obj1->radius + obj2->radius;

	const float distanceSquared = deltaX * deltaX + deltaY * deltaY;
	const float radiusSumSquared = deltaR * deltaR;

	if (distanceSquared >= radiusSumSquared) {
		return false;
	}

	const float angle = std::atan2(deltaY,deltaX);
	normal = { cosf(angle), sinf(angle) };
	depth = deltaR - std::sqrt(distanceSquared);

	if (normalPtr!=nullptr) 
	{
		(*normalPtr) = normal;
	}
	if (depthPtr!=nullptr)
	{
		(*depthPtr) = depth;
	}

	return true;
}

bool noa::PhysicsSystem::PolygonsCollide(
	Collider2D* obj1
	, Collider2D* obj2
	, noa::Vector<float>* normalPtr
	, float* depthPtr
)
{
	if (!obj1 || !obj2) {
		return false;
	}

	std::vector<noa::Vector<float>>& va = obj1->caculateVertices;
	std::vector<noa::Vector<float>>& vb = obj2->caculateVertices;

	const bool result = noa::Math::IntersectPolygons(
		va
		, vb
		, normalPtr
		, depthPtr);

	return result;

}

bool noa::PhysicsSystem::PolygonAndCircleCollide(
	Collider2D* obj1
	, Collider2D* obj2
	, noa::Vector<float>* normalPtr
	, float* depthPtr
)
{
	if (!obj1 || !obj2) 
	{
		return false;
	}

	noa::CircleCollider2D* circleCollider
		= (obj1->colliderType
			== noa::ColliderType::CIRCLE_COLLIDER)
		? dynamic_cast<noa::CircleCollider2D*>(obj1) 
		: dynamic_cast<noa::CircleCollider2D*>(obj2);
	
	noa::Collider2D* polygonCollider
		= (circleCollider == obj1) ? obj2 : obj1;

	const float radius = circleCollider->radius;
	const noa::Vector<float> circleCenter = 
		circleCollider->GetRigidbody()->newPosition 
		+ circleCollider->offset;
	const std::vector<noa::Vector<float>> vertices
		= polygonCollider->caculateVertices;

	return noa::Math::IntersectCirclePolygon(circleCenter, radius, vertices, normalPtr, depthPtr);

}
