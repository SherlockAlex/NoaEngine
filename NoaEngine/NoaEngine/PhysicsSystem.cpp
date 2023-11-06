#include "Collider2D.h"
#include "PhysicsSystem.h"
#include "Time.h"
#include "Physics.h"


noa::Grid noa::PhysicsSystem::grid;
std::vector<noa::Rigidbody*> noa::PhysicsSystem::rigidbodys;
noa::Vector<float> noa::PhysicsSystem::gravity = { 0,9.82f };

int noa::PhysicsSystem::step = 5;

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

		InitVelocity(subDeltaTime);			//计算速度
		ApplyTileCollision(subDeltaTime);	//通过新位置判断位置合不合理，不合理修改新位置还有修改速度
		ApplyTileConstraint(subDeltaTime);	//计算刚体是否处于约束
		ApplyVelocity(subDeltaTime);		//应用速度
		InitPosition(subDeltaTime);			//通过速度，计算新的位置
		ApplyTileCollision(subDeltaTime);	//通过新位置判断位置合不合理，不合理修改新位置还有修改速度
		ApplyTileConstraint(subDeltaTime);	//计算刚体是否处于约束
		FindCollisionsGrid();				//刚体之间的碰撞，新位置处理和新速度处理
		ApplyTileCollision(subDeltaTime);	//刚体和TileMap的碰撞处理
		ApplyTileConstraint(subDeltaTime);	//刚体的位置约束情况
		ApplyPosition(subDeltaTime);		//将处理后合理的新位置应用到刚体身上
		
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

		const Vector<float> direction = Vector<float>(cosf(angle), sinf(angle));
		const Vector<float> normal = Vector<float>(cosf(angle + 2*PI), sinf(angle + 2 * PI));

		const float distance = std::sqrtf(deltaX * deltaX + deltaY * deltaY);

		const float deltaR = std::abs(distance - sumRadius);

		const float fixX = deltaR * direction.x;
		const float fixY = deltaR * direction.y;

		
		
		//计算受力
		//刚刚好接触的时候，双方不发生任何受力
		//及双发的动量很小

		const float bounce1 = obj1->rigidbody->bounce;
		const float bounce2 = obj2->rigidbody->bounce;

		const float m1 = obj1->rigidbody->mass;
		const float m2 = obj2->rigidbody->mass;

		const float invSumMass = 1.0f / (m1 + m2);

		const noa::Vector<float> p1 = obj1->rigidbody->velocity * m1;
		const noa::Vector<float> p2 = obj2->rigidbody->velocity * m2;

		const noa::Vector<float> p = p1 + p2;

		const bool constraintX1 = obj1->rigidbody->constraint.x;
		const bool constraintY1 = obj1->rigidbody->constraint.y;
		const bool constraintX2 = obj2->rigidbody->constraint.x;
		const bool constraintY2 = obj2->rigidbody->constraint.y;

		//假设x和y方向上没有任何约束

		const noa::Vector<float> impulse1 = (p * m1 * invSumMass - p1) * 2.0f;
		const noa::Vector<float> impulse2 = (p * m2 * invSumMass - p2) * 2.0f;

		const noa::Vector<float> newP1 = (p * m1 * invSumMass * 2.0f - p1);
		const noa::Vector<float> newP2 = (p * m2 * invSumMass * 2.0f - p2);

		noa::Vector<float> finalVel1 = newP1 * obj1->rigidbody->invMass;
		noa::Vector<float> finalVel2 = newP2 * obj2->rigidbody->invMass;

		obj1->rigidbody->newPosition.x = x1 + 0.5f * fixX;
		obj1->rigidbody->newPosition.y = y1 + 0.5f * fixY;

		obj2->rigidbody->newPosition.x = x2 - 0.5f * fixX;
		obj2->rigidbody->newPosition.y = y2 - 0.5f * fixY;

		//会出现卡住的问题，其实本质上是对方可能以及处于一个约束的情况
		//因为质量相同的情况，会出现一个速度交换的问题
		
		if (constraintX1) 
		{
			finalVel2.x = -obj2->rigidbody->velocity.x* bounce2;
			if (std::abs(finalVel2.x)<0.01f) 
			{
				obj2->rigidbody->nextConstraint.x = true;
			}
			
		}

		if (constraintX2)
		{
			finalVel1.x = -obj1->rigidbody->velocity.x* bounce1;
			if (std::abs(finalVel1.x) < 0.01f)
			{
				obj1->rigidbody->nextConstraint.x = true;
			}
		}

		if (constraintY1)
		{
			finalVel2.y = -obj2->rigidbody->velocity.y* bounce2;
			if (std::abs(finalVel2.y) < 0.01f)
			{
				obj2->rigidbody->nextConstraint.y = true;
			}
		}

		if (constraintY2)
		{
			finalVel1.y = -obj1->rigidbody->velocity.y* bounce1;
			if (std::abs(finalVel1.y) < 0.01f)
			{
				obj1->rigidbody->nextConstraint.y = true;
			}
			
		}

		//同时给两个物体的接触面的切线方向添加一个速度

		obj1->rigidbody->velocity = finalVel1;
		obj2->rigidbody->velocity = finalVel2;

	}

	

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

void noa::PhysicsSystem::ApplyTileCollision(float deltaTime)
{
#pragma omp parallel for
	for (auto& rigidbody : rigidbodys)
	{
		if (rigidbody == nullptr)
		{
			continue;
		}

		rigidbody->ApplyTileCollision(deltaTime);

	}
}

void noa::PhysicsSystem::ApplyTileConstraint(float deltaTime)
{
#pragma omp parallel for
	for (auto& rigidbody : rigidbodys)
	{
		if (rigidbody == nullptr)
		{
			continue;
		}

		rigidbody->ApplyTileConstraint(deltaTime);

	}
}


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
					if (!collider1->isTrigger&&!collider2->isTrigger)
					{
						SolveCollision(collider1, collider2);
					}
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

	return distanceSquared < radiusSumSquared;
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
