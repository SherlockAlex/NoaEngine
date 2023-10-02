#include "TestRigidbody.h"

// No sub steps
void PhysicsManager::Update(float deltaTime)
{
	// Perform the update
}

// Using sub steps
void PhysicsManager::UpdateSubSteps(float deltaTime, int subSteps)
{
	const float sub_dt = deltaTime / float(subSteps);
	for (int i{subSteps};i--;)
	{
		Update(sub_dt);
	}
}

void PhysicsManager::FindCollisions()
{
	//�㷨ƿ�����������񻯼�����
	for (auto & collider1: physicsWorld)
	{
		for (auto & collider2:physicsWorld)
		{
			if (collider1 != collider2)
			{
				if (Collide(collider1,collider2)) 
				{
					SolveCollision(collider1,collider2);
				}
			}
		}
	}
}

void PhysicsManager::FindCollisionsGrid()
{
	// Skip left and right columns
	for (int x{1};x<grid.width-1;x++)
	{
		// Skip top and bottom rows
		for (int y{1};y<grid.height-1;y++)
		{
			auto& currentCell = grid.GetCell(x,y);
			// Iterate on all surrounding cells,including the current one

			for (int dx{-1};dx<=1;dx++)
			{
				for (int dy{-1};dy<=1;dy++)
				{
					auto& otherCell = grid.GetCell(x+dx,y+dy);
					CheckCellsCollisions(currentCell, otherCell);
				}
			}

		}
	}
}

bool PhysicsManager::Collide(CircleCollider2D* obj1, CircleCollider2D* obj2)
{
	// ������Բ��֮��ľ����ƽ��
	float dx = obj2->rigidbody->actor->transform.position.x - obj1->rigidbody->actor->transform.position.x;
	float dy = obj2->rigidbody->actor->transform.position.y - obj1->rigidbody->actor->transform.position.y;
	float distanceSquared = dx * dx + dy * dy;

	// ������Բ�뾶֮�͵�ƽ��
	float radiusSumSquared = (obj1->radius + obj2->radius) * (obj1->radius + obj2->radius);

	// ��������ƽ��С�ڵ��ڰ뾶֮�͵�ƽ����˵��������ײ
	return distanceSquared <= radiusSumSquared;
}

void PhysicsManager::SolveCollision(CircleCollider2D* obj1, CircleCollider2D* obj2)
{
	//�����������ж�Ϊ��ײ������
	// ������ײ����
	float dx = obj2->rigidbody->actor->transform.position.x - obj1->rigidbody->actor->transform.position.x;
	float dy = obj2->rigidbody->actor->transform.position.y - obj1->rigidbody->actor->transform.position.y;
	float length = std::sqrt(dx * dx + dy * dy);
	float collisionNormalX = dx / length;
	float collisionNormalY = dy / length;

	// �����ƿ����룬�����ʹ����Բ�뾶֮�ͼ�ȥ��Բ�ľ����һ��
	float penetration = (obj1->radius + obj2->radius) - length;

	// ����ײ���߳����ƿ����룬�ֱ��ƿ�����Բ
	obj1->rigidbody->actor->transform.position.x -= collisionNormalX * (penetration * 0.5f);
	obj1->rigidbody->actor->transform.position.y -= collisionNormalY * (penetration * 0.5f);
	obj2->rigidbody->actor->transform.position.x += collisionNormalX * (penetration * 0.5f);
	obj2->rigidbody->actor->transform.position.y += collisionNormalY * (penetration * 0.5f);
}

void PhysicsManager::CheckCellsCollisions(Cell& cell1, Cell& cell2)
{
	for (auto & collider1:cell1.colliders) 
	{
		for (auto & collider2:cell2.colliders)
		{
			if (collider1!=collider2) 
			{
				if (Collide(collider1, collider2))
				{
					SolveCollision(collider1, collider2);
				}
			}
		}
	}
}
