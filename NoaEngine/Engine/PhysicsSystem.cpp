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

			if (Collide(collider1, collider2))
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
					SolveCollision(collider1, collider2);
				}

				//两者其中一个是trigger

				collider1->ApplyTriggerEnter(*collider2);
				collider2->ApplyTriggerEnter(*collider1);

			}

		}
	}
}

bool noa::PhysicsSystem::Collide(Collider2D* obj1, Collider2D* obj2)
{

	//碰撞检测

	if (obj1->colliderType == ColliderType::CIRCLE_COLLIDER
		&&obj2->colliderType == ColliderType::CIRCLE_COLLIDER) 
	{
		return CircleCollide(
			dynamic_cast<CircleCollider2D*>(obj1)
			, dynamic_cast<CircleCollider2D*>(obj2)
		);
	}
	else if (obj1->colliderType == ColliderType::BOX_COLLIDER
		&&obj2->colliderType == ColliderType::BOX_COLLIDER) 
	{
		return BoxCollide(
			dynamic_cast<BoxCollider2D*>(obj1)
			, dynamic_cast<BoxCollider2D*>(obj2)
		);
	}
	else if (obj1->colliderType == ColliderType::BOX_COLLIDER 
		&& obj2->colliderType == ColliderType::CIRCLE_COLLIDER
		)
	{
		return BoxAndCircleCollide(
			dynamic_cast<BoxCollider2D*>(obj1)
			,dynamic_cast<CircleCollider2D*>(obj2)
		);
	}
	else if (obj1->colliderType == ColliderType::CIRCLE_COLLIDER 
		&& obj2->colliderType == ColliderType::BOX_COLLIDER
		)
	{
		return BoxAndCircleCollide(dynamic_cast<BoxCollider2D*>(obj2)
			, dynamic_cast<CircleCollider2D*>(obj1)
		);
	}
	return false;
	
}

void noa::PhysicsSystem::SolveCollision(Collider2D* obj1, Collider2D* obj2)
{
	//求解碰撞

	Rigidbody* rigid1 = obj1->rigidbody;
	Rigidbody* rigid2 = obj2->rigidbody;

	const float x1 = rigid1->newPosition.x + obj1->offset.x;
	const float y1 = rigid1->newPosition.y + obj1->offset.y;
	const float x2 = rigid2->newPosition.x + obj2->offset.x;
	const float y2 = rigid2->newPosition.y + obj2->offset.y;

	//修正位置
	if (obj1->colliderType == ColliderType::CIRCLE_COLLIDER
		&& obj2->colliderType == ColliderType::CIRCLE_COLLIDER)
	{
		//如果两个都是圆形
		CircleCollider2D* collider1 = obj1->GetCollider2DAs<CircleCollider2D>();
		CircleCollider2D* collider2 = obj2->GetCollider2DAs<CircleCollider2D>();

		const float sumRadius = collider1->radius + collider2->radius;

		const float deltaX = (x1 - x2);
		const float deltaY = (y1 - y2);

		const float angle = atan2(deltaY, deltaX);

		const Vector<float> normal = { cosf(angle), sinf(angle) };

		const float distance = std::sqrtf(deltaX * deltaX + deltaY * deltaY);

		const float deltaR = std::abs(distance - sumRadius);

		const float fixX = deltaR * normal.x;
		const float fixY = deltaR * normal.y;
		
		//计算受力

		const float bounce1 = rigid1->bounce;
		const float bounce2 = rigid2->bounce;

		rigid1->newPosition.x = x1 + 0.5f * fixX;
		rigid1->newPosition.y = y1 + 0.5f * fixY;

		rigid2->newPosition.x = x2 - 0.5f * fixX;
		rigid2->newPosition.y = y2 - 0.5f * fixY;
		
		//计算接触力

		const float beta = 150;//太小，扰动越明显，太大或剧烈跳动

		const Vector<float> constraintImpulse1 = {
			 beta*fixX
			,beta*fixY
		};
		const Vector<float> constraintImpulse2 = {
			-beta * fixX
			,-beta * fixY
		};

		rigid1->AddForce(
			constraintImpulse1, noa::ForceType::IMPULSE_FORCE);
		rigid2->AddForce(
			constraintImpulse2, noa::ForceType::IMPULSE_FORCE);

	}
	else if (obj1->colliderType == ColliderType::BOX_COLLIDER
		&& obj2->colliderType == ColliderType::BOX_COLLIDER)
	{
		//如果两个都是矩形
		BoxCollider2D* collider1 = obj1->GetCollider2DAs<BoxCollider2D>();
		BoxCollider2D* collider2 = obj2->GetCollider2DAs<BoxCollider2D>();

		const float w1 = collider1->size.x;
		const float h1 = collider1->size.y;
		const float w2 = collider2->size.x;
		const float h2 = collider2->size.y;

		//计算两个点重合的法相信息
		const float left1 = x1;
		const float right1 = x1 + w1;
		const float top1 = y1;
		const float bottom1 = y1 + h1;

		const float left2 = x2;
		const float right2 = x2 + w2;
		const float top2 = y2;
		const float bottom2 = y2 + h2;

		// 计算碰撞信息
		const float overlapX = 
			std::max(
				0.0f
				, std::min(right1, right2) 
				- std::max(left1, left2));
		const float overlapY = 
			std::max(
				0.0f
				, std::min(bottom1, bottom2) 
				- std::max(top1, top2));

		//计算出了overlap
		//判断相对速度方向(刚体1相对于刚体2)
		const noa::Vector<float> relativeVelocity 
			= rigid1->velocity - rigid2->velocity;
	}

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

bool noa::PhysicsSystem::CircleCollide(CircleCollider2D* obj1, CircleCollider2D* obj2)
{
	if (!obj1||!obj2) 
	{
		return false;
	}

	Rigidbody* rigid1 = obj1->rigidbody;
	Rigidbody* rigid2 = obj2->rigidbody;

	const float deltaX = rigid1->newPosition.x + obj1->offset.x
		- rigid2->newPosition.x - obj2->offset.x;

	const float deltaY = rigid1->newPosition.y + obj1->offset.y
		- rigid2->newPosition.y - obj2->offset.y;

	const float deltaR = obj1->radius + obj2->radius;

	const float distanceSquared = deltaX * deltaX + deltaY * deltaY;
	const float radiusSumSquared = deltaR * deltaR;

	return distanceSquared < radiusSumSquared;
}

bool noa::PhysicsSystem::BoxCollide(BoxCollider2D* obj1, BoxCollider2D* obj2)
{
	if (!obj1 || !obj2) {
		return false;
	}

	const float obj1X = obj1->rigidbody->newPosition.x + obj1->offset.x;
	const float obj1Y = obj1->rigidbody->newPosition.y + obj1->offset.y;
	const float obj1Width = obj1->size.x;
	const float obj1Height = obj1->size.y;

	const float obj2X = obj2->rigidbody->newPosition.x + obj2->offset.x;
	const float obj2Y = obj2->rigidbody->newPosition.y + obj2->offset.y;
	const float obj2Width = obj2->size.x;
	const float obj2Height = obj2->size.y;

	const float obj1MinX = obj1X;
	const float obj1MaxX = obj1X + obj1Width;
	const float obj1MinY = obj1Y;
	const float obj1MaxY = obj1Y + obj1Height;

	const float obj2MinX = obj2X;
	const float obj2MaxX = obj2X + obj2Width;
	const float obj2MinY = obj2Y;
	const float obj2MaxY = obj2Y + obj2Height;

	if (obj1MaxX<obj2MinX||obj1MinX>obj2MaxX) 
	{
		return false;
	}
	if (obj1MaxY<obj2MinY || obj1MinY>obj2MaxY)
	{
		return false;
	}
	return true;

}

bool noa::PhysicsSystem::BoxAndCircleCollide(BoxCollider2D* obj1, CircleCollider2D* obj2)
{
	if (!obj1 || !obj2) {
		return false;
	}

	float rectX = obj1->rigidbody->newPosition.x;
	float rectY = obj1->rigidbody->newPosition.y;
	float rectWidth = obj1->size.x;
	float rectHeight = obj1->size.y;

	float circleX = obj2->rigidbody->newPosition.x;
	float circleY = obj2->rigidbody->newPosition.y;
	float circleRadius = obj2->radius;

	float rectHalfWidth = rectWidth / 2;
	float rectHalfHeight = rectHeight / 2;

	float rectCenterX = rectX + rectHalfWidth;
	float rectCenterY = rectY + rectHalfHeight;

	float deltaX = abs(circleX - rectCenterX);
	float deltaY = abs(circleY - rectCenterY);

	if (deltaX > (rectHalfWidth + circleRadius)) {
		return false;
	}
	if (deltaY > (rectHalfHeight + circleRadius)) {
		return false;
	}

	if (deltaX <= rectHalfWidth || deltaY <= rectHalfHeight) {
		return true;
	}

	float cornerDistanceSquared = powf(deltaX - rectHalfWidth, 2) + powf(deltaY - rectHalfHeight, 2);
	return cornerDistanceSquared <= (circleRadius * circleRadius);
}
