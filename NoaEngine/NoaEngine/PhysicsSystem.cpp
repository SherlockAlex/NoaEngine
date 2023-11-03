#include "Collider2D.h"
#include "PhysicsSystem.h"
#include "Time.h"
#include "Physics.h"


noa::Grid noa::PhysicsSystem::grid;
std::vector<noa::Rigidbody*> noa::PhysicsSystem::rigidbodys;
noa::Vector<float> noa::PhysicsSystem::gravity = { 0,9.82f };

int noa::PhysicsSystem::step = 20;

void noa::PhysicsSystem::SetGrid(int width, int height)
{
	grid.cells.clear();
	if (width * height == 0)
	{
		return;
	}
	grid.width = width;
	grid.height = height;
	for (int i = 0;i<width * height;i++)
	{
		Cell cell;
		cell.colliders.clear();
		grid.cells.push_back(cell);
	}
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
		//对位置进行修正
		FixTileCollisionPosition(subDeltaTime);
		FindCollisionsGrid();
		FixTileCollisionPosition(subDeltaTime);
		ApplyPosition(subDeltaTime);
		
	}
	

	for (auto& cell : grid.cells)
	{
		cell.colliders.clear();
	}
	rigidbodys.clear();
}

void noa::PhysicsSystem::FindCollisionsGrid()
{
#pragma omp parallel for
	for (int x{ 1 }; x < grid.width - 1; x++)
	{
		for (int y{ 1 }; y < grid.height - 1; y++)
		{
			auto* currentCell = grid.GetCell(x, y);

			for (int dx{ -1 }; dx <= 1; dx++)
			{
				for (int dy{ -1 }; dy <= 1; dy++)
				{
					auto* otherCell = grid.GetCell(x + dx, y + dy);
					CheckCellsCollisions(*currentCell, *otherCell);
				}
			}

		}
	}

}

bool noa::PhysicsSystem::Collide(Collider2D* obj1, Collider2D* obj2)
{
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

bool noa::PhysicsSystem::Collide(const AABB& box1, const AABB& box2)
{
	if (box1.maxX < box2.minX || box1.minX > box2.maxX) {
		return false;
	}
	if (box1.maxY < box2.minY || box1.minY > box2.maxY) {
		return false;
	}
	return true;
}

void noa::PhysicsSystem::SolveCollision(Collider2D* obj1, Collider2D* obj2)
{

	//修正位置
	if (obj1->colliderType == ColliderType::CIRCLE_COLLIDER
		&& obj2->colliderType == ColliderType::CIRCLE_COLLIDER)
	{
		//对当前位置进行修正
		CircleCollider2D* collider1 = obj1->GetCollider2DAs<CircleCollider2D>();
		CircleCollider2D* collider2 = obj2->GetCollider2DAs<CircleCollider2D>();

		const float sumRadius = collider1->radius + collider2->radius;

		const float x1 = collider1->rigidbody->newPosition.x;
		const float y1 = collider1->rigidbody->newPosition.y;
		const float x2 = collider2->rigidbody->newPosition.x;
		const float y2 = collider2->rigidbody->newPosition.y;

		const float deltaX = (x1 - x2);
		const float deltaY = (y1 - y2);

		const float angle = atan2(deltaY, deltaX);

		const float distance = std::sqrtf(deltaX * deltaX + deltaY * deltaY);

		const float deltaR = std::abs(distance - sumRadius);

		const float fixX = 0.5f * deltaR * cosf(angle);
		const float fixY = 0.5f * deltaR * sinf(angle);

		obj1->rigidbody->newPosition.x = x1 + fixX;
		obj1->rigidbody->newPosition.y = y1 + fixY;

		obj2->rigidbody->newPosition.x = x2 - fixX;
		obj2->rigidbody->newPosition.y = y2 - fixY;
		
		//计算受力
		//刚刚好接触的时候，双方不发生任何受力
		//及双发的动量很小

		const float m1 = obj1->rigidbody->mass;
		const float m2 = obj2->rigidbody->mass;

		const float invSumMass = 1.0f / (m1 + m2);

		const noa::Vector<float> p1 = obj1->rigidbody->GetMomentum();
		const noa::Vector<float> p2 = obj2->rigidbody->GetMomentum();

		const noa::Vector<float> p = p1 + p2;

		//弹性碰撞
		const noa::Vector<float> deltaP1 = (p * m1 * invSumMass - p1) * 2.0f;
		const noa::Vector<float> deltaP2 = { -deltaP1.x,-deltaP1.y };

		//计算损失

		const float currentX1 = obj1->GetActor()->transform.position.x;
		const float currentY1 = obj1->GetActor()->transform.position.y;
		const float currentX2 = obj2->GetActor()->transform.position.x;
		const float currentY2 = obj2->GetActor()->transform.position.y;

		const float currentDeltaX = currentX1 - currentX2;
		const float currentDeltaY = currentY1 - currentY2;

		const float currentDistance = std::sqrtf(currentDeltaX * currentDeltaX 
			+ currentDeltaY * currentDeltaY);

		const float overlap = (currentDistance > sumRadius) ?0.8f : (currentDistance / sumRadius);

		const noa::Vector<float> impulse = deltaP1*overlap;

		obj1->rigidbody->AddForce(impulse, ForceType::IMPULSE_FORCE);
		obj2->rigidbody->AddForce(impulse*(-1.0f), ForceType::IMPULSE_FORCE);

	}

	

}

noa::Vector<float> noa::PhysicsSystem::CaculateCollideForce(Collider2D* obj1, Collider2D* obj2)
{
	if (obj1->colliderType == ColliderType::CIRCLE_COLLIDER
		&& obj2->colliderType == ColliderType::CIRCLE_COLLIDER)
	{
		CircleCollider2D* collider1 = obj1->GetCollider2DAs<CircleCollider2D>();
		CircleCollider2D* collider2 = obj2->GetCollider2DAs<CircleCollider2D>();

		Vector<float> force;

		const float relativeX = obj1->rigidbody->newPosition.x 
			- obj2->rigidbody->newPosition.x;
		const float relativeY = obj1->rigidbody->newPosition.y
			- obj2->rigidbody->newPosition.y;

		const float distance = std::sqrtf(relativeX * relativeX 
			+ relativeY * relativeY);

		const float angle = atan2(relativeY,relativeX);

		const float normalX = distance * cosf(angle);
		const float normalY = distance * sinf(angle);

		const float relativeVelocityX = obj1->rigidbody->velocity.x
			- obj2->rigidbody->velocity.x;
		const float relativeVelocityY = obj1->rigidbody->velocity.y
			- obj2->rigidbody->velocity.y;

		const float m1 = obj1->rigidbody->mass;
		const float m2 = obj2->rigidbody->mass;

		float impulse = (2.0f * (m1 * m2) * (relativeVelocityX * normalX + relativeVelocityY * normalY))
			/ ((m1 + m2) * distance);

		force.x = impulse * normalX;
		force.y = impulse * normalY;
		return force;

	}
	return noa::Vector<float>();
}

void noa::PhysicsSystem::InitVelocity(float deltaTime)
{
#pragma omp parallel for
	for (auto& rigidbody : rigidbodys)
	{
		if (rigidbody == nullptr)
		{
			continue;
		}
		rigidbody->InitVelocity(deltaTime);
	}
}

void noa::PhysicsSystem::ApplyVelocity(float deltaTime)
{
#pragma omp parallel for
	for (auto& rigidbody : rigidbodys)
	{
		if (rigidbody == nullptr)
		{
			continue;
		}

		rigidbody->ApplyVelocity(deltaTime);

	}
}

void noa::PhysicsSystem::InitPosition(float deltaTime)
{
#pragma omp parallel for
	for (auto& rigidbody : rigidbodys)
	{
		if (rigidbody == nullptr)
		{
			continue;
		}

		rigidbody->InitPosition(deltaTime);

	}
}

void noa::PhysicsSystem::FixTileCollisionVelocity(float deltaTime)
{
#pragma omp parallel for
	for (auto& rigidbody : rigidbodys)
	{
		if (rigidbody == nullptr)
		{
			continue;
		}

		rigidbody->ApplyTileFixVelocity();

	}
}

void noa::PhysicsSystem::FixTileCollisionPosition(float deltaTime)
{
#pragma omp parallel for
	for (auto& rigidbody : rigidbodys)
	{
		if (rigidbody == nullptr)
		{
			continue;
		}

		//rigidbody->ApplyTileFixPosition();
		rigidbody->ApplyTileCollision(deltaTime);

	}
}

//void noa::PhysicsSystem::CaculateTileCollision(float deltaTime)
//{
//#pragma omp parallel for
//	for (auto& rigidbody : rigidbodys)
//	{
//		if (rigidbody == nullptr)
//		{
//			continue;
//		}
//
//		rigidbody->ApplyTileCollision();
//
//	}
//}

void noa::PhysicsSystem::ApplyPosition(float deltaTime)
{
#pragma omp parallel for
	for (auto& rigidbody : rigidbodys)
	{
		if (rigidbody == nullptr)
		{
			continue;
		}

		rigidbody->ApplyPositon(deltaTime);

	}
}

void noa::PhysicsSystem::CheckCellsCollisions(Cell& cell1, Cell& cell2)
{
	
#pragma omp parallel for
	for (auto& collider1 : cell1.colliders)
	{
		for (auto& collider2 : cell2.colliders)
		{
			if (collider1 != collider2)
			{
				if (Collide(collider1, collider2))
				{
					const bool isAllTrigger = collider1->isTrigger
						&& collider2->isTrigger;
					if (isAllTrigger) 
					{
						continue;
					}
					collider1->rigidbody->collision.actor = collider2->GetActor();
					collider2->rigidbody->collision.actor = collider1->GetActor();
					SolveCollision(collider1, collider2);
					collider1->ApplyTrigger();
					collider2->ApplyTrigger();

				}
			}
		}
	}
}

bool noa::PhysicsSystem::CircleCollide(CircleCollider2D* obj1, CircleCollider2D* obj2)
{
	if (!obj1||!obj2) 
	{
		return false;
	}

	const float deltaX = obj1->rigidbody->newPosition.x
		- obj2->rigidbody->newPosition.x;

	const float deltaY = obj1->rigidbody->newPosition.y
		- obj2->rigidbody->newPosition.y;

	const float deltaR = obj1->radius + obj2->radius;

	const float distanceSquared = deltaX * deltaX + deltaY * deltaY;
	const float radiusSumSquared = deltaR * deltaR;

	return distanceSquared <= radiusSumSquared;
}

bool noa::PhysicsSystem::BoxCollide(BoxCollider2D* obj1, BoxCollider2D* obj2)
{
	if (!obj1 || !obj2) {
		return false;
	}

	float obj1X = obj1->rigidbody->newPosition.x;
	float obj1Y = obj1->rigidbody->newPosition.y;
	float obj1Width = obj1->scale.x;
	float obj1Height = obj1->scale.y;

	float obj2X = obj2->rigidbody->newPosition.x;
	float obj2Y = obj2->rigidbody->newPosition.y;
	float obj2Width = obj2->scale.x;
	float obj2Height = obj2->scale.y;

	float obj1Left = obj1X - obj1Width / 2;
	float obj1Right = obj1X + obj1Width / 2;
	float obj1Top = obj1Y + obj1Height / 2;
	float obj1Bottom = obj1Y - obj1Height / 2;

	float obj2Left = obj2X - obj2Width / 2;
	float obj2Right = obj2X + obj2Width / 2;
	float obj2Top = obj2Y + obj2Height / 2;
	float obj2Bottom = obj2Y - obj2Height / 2;

	if (obj1Right < obj2Left 
		|| obj1Left > obj2Right 
		|| obj1Top < obj2Bottom 
		|| obj1Bottom > obj2Top) {
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
	float rectWidth = obj1->scale.x;
	float rectHeight = obj1->scale.y;

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
