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
			rigidbody->Update(subDeltaTime);

			rigidbody->ApplyTrigger();
			rigidbody->LateUpdate(subDeltaTime);
		}
		
		/*for (auto& rigidbody : rigidbodys)
		{
			
		}*/
	}
	

	for (auto& cell : grid.cells)
	{
		cell.colliders.clear();
	}
	rigidbodys.clear();
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

	const float deltaX = obj1->rigidbody->actor->transform.position.x 
		- obj2->rigidbody->newPosition.x;

	const float deltaY = obj1->rigidbody->actor->transform.position.y 
		- obj2->rigidbody->newPosition.y;

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

	const float m1 = obj1->rigidbody->mass;
	const float m2 = obj2->rigidbody->mass;

	const float invSumMass = 1.0f/(m1 + m2);

	const noa::Vector<float> v1 = obj1->rigidbody->velocity;
	const noa::Vector<float> v2 = obj2->rigidbody->velocity;

	const noa::Vector<float> p1 = v1 * m1;
	const noa::Vector<float> p2 = v2 * m2;

	const noa::Vector<float> p = p1 + p2;

	const noa::Vector<float> f1 = ((p * m1 * invSumMass - p1) * (2.0f / noa::Time::deltaTime))*4.0f;
	const noa::Vector<float> f2 = {-f1.x,-f1.y};

	obj1->rigidbody->AddForce(f1, noa::ForceType::CONTINUOUS_FORCE);
	obj2->rigidbody->AddForce(f2, noa::ForceType::CONTINUOUS_FORCE);

	//// 处理两个被判定为碰撞的物体

	//const float dx = obj2->rigidbody->newPosition.x - obj1->rigidbody->actor->transform.position.x;
	//const float dy = obj2->rigidbody->newPosition.y - obj1->rigidbody->actor->transform.position.y;
	//const float length = std::sqrt(dx * dx + dy * dy);
	//const float collisionNormalX = (length == 0) ? 0 : (dx / length);
	//const float collisionNormalY = (length == 0) ? 0 : (dy / length);

	//// 计算推开距离,碰撞的物体马上停下来
	//const float penetration = (obj1->radius + obj2->radius) - length;

	//obj2->rigidbody->newPosition.x += collisionNormalX * (penetration);
	//obj2->rigidbody->newPosition.y += collisionNormalY * (penetration);

	//obj2->rigidbody->velocity = { 0,0 };
	//obj1->rigidbody->velocity = { 0,0 };

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
