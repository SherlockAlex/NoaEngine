#include "NoaEngine.h"

noa::Grid noa::PhysicsSystem::grid;
std::vector<noa::Rigidbody*> noa::PhysicsSystem::rigidbodys;

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
	for (int i{step};i--;)
	{
		FindCollisionsGrid();

		for (auto& rigidbody : rigidbodys)
		{
			if (rigidbody == nullptr)
			{
				continue;
			}
			rigidbody->UpdateVelocity(subDeltaTime);
			rigidbody->UpdatePosition(subDeltaTime);
		}
		
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

void noa::PhysicsSystem::SolveCollision(Collider2D* obj1, Collider2D* obj2)
{
	if (obj1->isTrigger||obj2->isTrigger)
	{
		return;
	}

	//如果两者夹在一起

	const float m1 = obj1->rigidbody->mass;
	const float m2 = obj2->rigidbody->mass;

	const float invSumMass = 1.0f/(m1 + m2);

	const noa::Vector<float> & v1 = obj1->rigidbody->velocity;
	const noa::Vector<float> & v2 = obj2->rigidbody->velocity;

	const noa::Vector<float> & p1 = v1 * m1;
	const noa::Vector<float> & p2 = v2 * m2;

	const noa::Vector<float> & p = p1 + p2;

	const noa::Vector<float> & f1 = ((p * m1 * invSumMass - p1) * (2.0f / noa::Time::deltaTime))*3.0f;
	const noa::Vector<float> & f2 = {-f1.x,-f1.y};

	obj1->rigidbody->AddForce(f1, noa::ForceType::CONTINUOUS_FORCE);
	obj2->rigidbody->AddForce(f2, noa::ForceType::CONTINUOUS_FORCE);

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
					collider1->rigidbody->collision.actor = collider2->actor;
					collider2->rigidbody->collision.actor = collider1->actor;
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

	const float deltaX = obj1->rigidbody->actor->transform.position.x
		- obj2->rigidbody->actor->transform.position.x;

	const float deltaY = obj1->rigidbody->actor->transform.position.y
		- obj2->rigidbody->actor->transform.position.y;

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

	float obj1X = obj1->rigidbody->actor->transform.position.x;
	float obj1Y = obj1->rigidbody->actor->transform.position.y;
	float obj1Width = obj1->scale.x;
	float obj1Height = obj1->scale.y;

	float obj2X = obj2->rigidbody->actor->transform.position.x;
	float obj2Y = obj2->rigidbody->actor->transform.position.y;
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

	if (obj1Right < obj2Left || obj1Left > obj2Right || obj1Top < obj2Bottom || obj1Bottom > obj2Top) {
		return false;
	}

	return true;
}

bool noa::PhysicsSystem::BoxAndCircleCollide(BoxCollider2D* obj1, CircleCollider2D* obj2)
{
	if (!obj1 || !obj2) {
		return false;
	}

	float rectX = obj1->rigidbody->actor->transform.position.x;
	float rectY = obj1->rigidbody->actor->transform.position.y;
	float rectWidth = obj1->scale.x;
	float rectHeight = obj1->scale.y;

	float circleX = obj2->rigidbody->actor->transform.position.x;
	float circleY = obj2->rigidbody->actor->transform.position.y;
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
