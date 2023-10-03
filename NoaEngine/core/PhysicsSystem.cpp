#include "NoaEngine.h"

noa::Grid noa::PhysicsSystem::grid;

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

void noa::PhysicsSystem::Update()
{
	FindCollisionsGrid();

	for (auto& cell : grid.cells)
	{
		cell.colliders.clear();
	}
}

void noa::PhysicsSystem::FindCollisionsGrid()
{
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

bool noa::PhysicsSystem::Collide(CircleCollider2D* obj1, CircleCollider2D* obj2)
{

	//Rigidbody* body2 = obj2->rigidbody;
	const float deltaX = obj1->rigidbody->actor->transform.position.x 
		- obj2->rigidbody->actor->transform.position.x;

	const float deltaY = obj1->rigidbody->actor->transform.position.y 
		- obj2->rigidbody->actor->transform.position.y;

	const float deltaR = obj1->radius + obj2->radius;

	const float distanceSquared = deltaX * deltaX + deltaY * deltaY;
	const float radiusSumSquared = deltaR * deltaR;

	// 如果两个球型包围盒的距离的平方小于半径之和的平方，则它们相交
	return distanceSquared < radiusSumSquared;
}

void noa::PhysicsSystem::SolveCollision(CircleCollider2D* obj1, CircleCollider2D* obj2)
{
	//该方法只处理碰撞，比如说两者的碰撞瞬间的动量问题
	if (obj1->rigidbody->collision.isTrigger||obj2->rigidbody->collision.isTrigger)
	{
		return;
	}

	// 处理两个被判定为碰撞的物体

	const float dx = obj2->rigidbody->newPosition.x - obj1->rigidbody->actor->transform.position.x;
	const float dy = obj2->rigidbody->newPosition.y - obj1->rigidbody->actor->transform.position.y;
	const float length = std::sqrt(dx * dx + dy * dy);
	const float collisionNormalX = (length == 0) ? 0 : (dx / length);
	const float collisionNormalY = (length == 0) ? 0 : (dy / length);

	// 计算推开距离,碰撞的物体马上停下来
	const float penetration = (obj1->radius + obj2->radius) - length;

	obj2->rigidbody->newPosition.x += 0.499*collisionNormalX * (penetration);
	obj2->rigidbody->newPosition.y += 0.499*collisionNormalY * (penetration);

	obj2->rigidbody->velocity = { 0,0 };
	obj1->rigidbody->velocity = { 0,0 };

}

void noa::PhysicsSystem::CheckCellsCollisions(Cell& cell1, Cell& cell2)
{
	
	for (auto& collider1 : cell1.colliders)
	{
		for (auto& collider2 : cell2.colliders)
		{
			if (collider1 != collider2)
			{
				if (Collide(collider1, collider2))
				{
					collider1->rigidbody->collision.other = collider2->rigidbody;
					collider2->rigidbody->collision.other = collider1->rigidbody;
					SolveCollision(collider1, collider2);
				}
			}
		}
	}
}
