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

		InitVelocity(subDeltaTime);
		//ApplyTileCollision(subDeltaTime);
		//ApplyTileConstraint(subDeltaTime);
		ApplyVelocity(subDeltaTime);
		InitPosition(subDeltaTime);
		//��λ�ý�������
		ApplyTileCollision(subDeltaTime);
		ApplyTileConstraint(subDeltaTime);
		FindCollisionsGrid();
		ApplyTileCollision(subDeltaTime);
		ApplyTileConstraint(subDeltaTime);
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

	//����λ��
	if (obj1->colliderType == ColliderType::CIRCLE_COLLIDER
		&& obj2->colliderType == ColliderType::CIRCLE_COLLIDER)
	{
		//�Ե�ǰλ�ý�������
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

		const Vector<float> normal = Vector<float>(cosf(angle), sinf(angle));

		const float distance = std::sqrtf(deltaX * deltaX + deltaY * deltaY);

		const float deltaR = std::abs(distance - sumRadius);

		const float fixX = deltaR * normal.x;
		const float fixY = deltaR * normal.y;

		
		
		//��������
		//�ոպýӴ���ʱ��˫���������κ�����
		//��˫���Ķ�����С

		const float m1 = obj1->rigidbody->mass;
		const float m2 = obj2->rigidbody->mass;

		const float invSumMass = 1.0f / (m1 + m2);

		const noa::Vector<float> p1 = obj1->rigidbody->velocity * m1;
		const noa::Vector<float> p2 = obj2->rigidbody->velocity * m2;

		const noa::Vector<float> p = p1 + p2;

		//������ײ
		const float bounce = 1.0f;

		const bool constraintX1 = obj1->rigidbody->constraint.x;
		const bool constraintY1 = obj1->rigidbody->constraint.y;
		const bool constraintX2 = obj2->rigidbody->constraint.x;
		const bool constraintY2 = obj2->rigidbody->constraint.y;

		//����x��y������û���κ�Լ��
		//˫����û���κ�Լ��

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

		//����ֿ�ס�����⣬��ʵ�������ǶԷ������Լ�����һ��Լ�������
		//��Ϊ������ͬ������������һ���ٶȽ���������

		if (constraintX1) 
		{
			finalVel2.x = - obj2->rigidbody->velocity.x;
			obj2->rigidbody->constraint.x = true;
		}

		if (constraintX2)
		{
			finalVel1.x = -obj1->rigidbody->velocity.x;
			obj1->rigidbody->constraint.x = true;
		}

		if (constraintY1)
		{
			finalVel2.y = -obj2->rigidbody->velocity.y;
			obj2->rigidbody->constraint.y = true;
		}

		if (constraintY2)
		{
			finalVel1.y = -obj1->rigidbody->velocity.y;
			obj1->rigidbody->constraint.y = true;
		}

		obj1->rigidbody->velocity = finalVel1;
		obj2->rigidbody->velocity = finalVel2;

		//obj1->rigidbody->AddForce(impulse1,ForceType::IMPULSE_FORCE);
		//obj2->rigidbody->AddForce(impulse2,ForceType::IMPULSE_FORCE);

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
