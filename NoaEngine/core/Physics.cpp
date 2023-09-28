#include "Physics.h"
#include "NoaEngine.h"
#include "Scene.h"
#include <unordered_map>
#include <thread>
#include "Actor.h"
#include <stack>

using namespace std;

namespace noa 
{

	extern float deltaTime;
	//检测rigid是否和其他刚体相撞，如果相撞就返回
	extern bool CollisionWithRigidbody(Rigidbody* rigid);
	extern bool BoundingBoxIntersect(Rigidbody* a, Rigidbody* b);
	// 判断两个球型包围盒是否相交
	extern bool IsColliding(Rigidbody* body1,Rigidbody* body2);
	float gravityAcceration = 9.81;
	
	unordered_map<size_t, bool> isCheckCollision;

	Rigidbody::Rigidbody(Actor * actor)
	{

		this->isRemoved = false;

		id = GetNextId();
		collision.other = nullptr;
		this->actor = actor;
		this->velocity = { 0,0 };
		invMass = 1.0 / mass;
		actor->GetActiveScene()->AddRigidbody(this);
		actor->AddRigidbody(this);
	}

	// 初始化静态计数器
	size_t Rigidbody::nextId = 0;

	Rigidbody::~Rigidbody()
	{
		Debug("Destory rigidBody");
		if (actor!=nullptr) 
		{
			actor->RemoveRigidbody();
		}
	}

	void Rigidbody::Start()
	{

	}

	//实现物理效果
	void Rigidbody::Update()
	{

		if (!active) 
		{
			return;
		}

		if (useMotion&&useGravity&&(!collision.isGrounded)) 
		{
			velocity.y += gravityWeight * gravityAcceration * deltaTime;
		}

		if (useMotion)
		{

			//处理力和速度的关系
			//F = ma
			velocity = (velocity * (1 - damping)) + (sumForce * (deltaTime * invMass));
			newPosition = (this->actor->transform.position) + (velocity * deltaTime);
			collision.isGrounded = false;
			collision.hitTileID = -1;
			ApplyCollision();
			//处理碰撞检测
			CollisionWithinRigidbody();
			ApplyCollision();
		}

		//更新位置

		if (useMotion && (!isFrozen))
		{
			this->actor->transform.position = newPosition;
		}

		if (collision.isTrigger && collision.other != nullptr && this->actor != nullptr)
		{
			this->actor->OnTrigger(collision);
		}
		collision.other = nullptr;

		if (collision.isHitCollisionTile&& this->actor!=nullptr)
		{
			this->actor->OnHitTile();
		}
		collision.isHitCollisionTile = false;

	}

	void Rigidbody::AddForce(const Vector<float> force, ForceType forceType)
	{
		//添加力到物体上
		switch (forceType)
		{
		case Rigidbody::ContinuousForce:
			//添加恒力到物体上
			sumForce += force;
			break;
		case Rigidbody::Impulse:
			//添加一个冲量到物体上，作用完马上就消失
			velocity += force * invMass;
			break;
		default:
			break;
		}

	}

	void Rigidbody::SetTileMap(TileMap* map)
	{
		this->tileMap = map;
		if (this->tileMap==nullptr)
		{
			Debug("Load tile map failed");
			exit(-1);
		}
	}

	Vector<float> pos(0.0, 0.0);
	void Rigidbody::ApplyCollision()
	{
		if (!useCollision||tileMap == nullptr)
		{
			return;
		}

		const float scaleX = collision.sacle.x;
		const float scaleY = collision.sacle.y;

		if (tileMap->IsCollisionTile(newPosition.x - scaleX, this->actor->transform.position.y - scaleY)
			|| tileMap->IsCollisionTile(newPosition.x - scaleX, this->actor->transform.position.y + 0.999 + scaleY)
		)
		{
			collision.isHitCollisionTile = true;
			if (!collision.isTrigger)
			{
				
				newPosition.x = (int)newPosition.x + 1 + scaleX;
				velocity.x = 0;
			}
		}

		if (tileMap->IsCollisionTile(newPosition.x + 0.999 + scaleX, this->actor->transform.position.y - scaleY)
			|| tileMap->IsCollisionTile(newPosition.x + 0.999 + scaleX, this->actor->transform.position.y + 0.999 + scaleY)
		)
		{
			collision.isHitCollisionTile = true;
			if (!collision.isTrigger)
			{
				
				newPosition.x = (int)newPosition.x - scaleX;
				velocity.x = 0;
			}

		}

		if (tileMap->IsCollisionTile(newPosition.x - scaleX, newPosition.y - scaleY)
			|| tileMap->IsCollisionTile(newPosition.x + 0.999 + scaleX, newPosition.y - scaleY)
		)
		{
			collision.isHitCollisionTile = true;
			if (!collision.isTrigger)
			{
				
				newPosition.y = (int)newPosition.y + 1 + scaleY;
				velocity.y = 0;
			}
		}

		if (tileMap->IsCollisionTile(newPosition.x - scaleX, newPosition.y + 0.999 + scaleY)
			|| tileMap->IsCollisionTile(newPosition.x + 0.999f + scaleX, newPosition.y + 0.999 + scaleY)
		)
		{
			collision.isHitCollisionTile = true;
			if (!collision.isTrigger) {
				collision.isGrounded = true;
				
				newPosition.y = (int)newPosition.y - scaleY;
				velocity.y = 0;
			}
		}

	}

	

	TileMap* Rigidbody::GetTileMap()
	{
		return this->tileMap;
	}

	Rigidbody* Rigidbody::Create(Actor* actor)
	{
		return new Rigidbody(actor);
	}

	void Rigidbody::Destroy()
	{
		Debug("Remove rigidbody");
		//会出现这个已经被销毁，但还是被访问
		this->isRemoved = true;
		this->actor->GetActiveScene()->RemoveRigidbody(this);
	}

	int Rigidbody::GetIndexInMap() const
	{
		return this->indexInMap;
	}

	void Rigidbody::UpdatePosition()
	{
		if (useMotion && (!isFrozen))
		{
			ApplyCollision();
			this->actor->transform.position = newPosition;

			//根据速度进行物体的碰撞检测
		//如果检测到了碰撞字符，就停止
			

		}
	}

	void Rigidbody::SetActive(bool value)
	{
		active = value;
	}

	bool Rigidbody::GetActive()
	{
		return active;
	}

	bool Rigidbody::CollisionWithinRigidbody()
	{

		if (!this->active|| this->actor->GetActiveScene() == nullptr||this->isRemoved)
		{
			return false;
		}

		const size_t hashCode = this->GetHashCode();
		const float rigidRadius = this->collision.radius;
		const float rigidX = this->newPosition.x;
		const float rigidY = this->newPosition.y;

		bool collisionOccurred = false;

		Scene* scene = this->actor->GetActiveScene();

		for (const auto& e : scene->rigidbodys)
		{
			if (e.first == hashCode|| e.second == nullptr||!e.second->GetActive()||e.second->isRemoved)
			{
				continue;
			}

			Rigidbody* rigidbody = e.second;

			// 检查包围盒是否相交
			if (!IsColliding(this, rigidbody))
			{
				continue;
			}

			// 检测两个刚体是否会相撞（使用平方距离）
			const float deltaX = rigidX - rigidbody->actor->transform.position.x;
			const float deltaY = rigidY - rigidbody->actor->transform.position.y;
			const float distanceSquared = deltaX * deltaX + deltaY * deltaY;
			const float radiusSumSquared = (rigidRadius + rigidbody->collision.radius) * (rigidRadius + rigidbody->collision.radius);

			if (distanceSquared >= radiusSumSquared)
			{
				continue;
			}

			this->collision.other = rigidbody;

			if (this->collision.isTrigger || rigidbody->collision.isTrigger || rigidbody->isFrozen)
			{
				return false;
			}

			// 处理碰撞后的结果
			const float distance = sqrt(distanceSquared);
			const float dr = sqrt(radiusSumSquared) - distance;
			const float dx = distance == 0?0:(dr * deltaX / distance);
			const float dy = distance == 0?0:(dr * deltaY / distance);

			this->newPosition.x += dx;
			this->newPosition.y += dy;

			this->velocity = { 0,0 };

			collisionOccurred = true;
		}

		return collisionOccurred;
	}

	bool CollisionWithRigidbody(Rigidbody* rigid)
	{
		if (sceneManager.GetActiveScene() == nullptr||rigid == nullptr || rigid->isFrozen)
		{
			return false;
		}

		const size_t hashCode = rigid->GetHashCode();
		const float rigidRadius = rigid->collision.radius;
		const float rigidX = rigid->newPosition.x;
		const float rigidY = rigid->newPosition.y;
		Vector<float>& rigidVelocity = rigid->velocity;

		bool collisionOccurred = false;

		for (const auto& e : sceneManager.GetActiveScene()->rigidbodys)
		{
			if (e.first == hashCode || e.second == nullptr)
			{
				continue;
			}

			Rigidbody* rigidbody = e.second;

			// 检查包围盒是否相交
			if (!BoundingBoxIntersect(rigid, rigidbody))
			{
				continue;
			}

			// 检测两个刚体是否会相撞（使用平方距离）
			const float deltaX = rigidX - rigidbody->actor->transform.position.x;
			const float deltaY = rigidY - rigidbody->actor->transform.position.y;
			const float distanceSquared = deltaX * deltaX + deltaY * deltaY;
			const float radiusSumSquared = (rigidRadius + rigidbody->collision.radius) * (rigidRadius + rigidbody->collision.radius);

			if (distanceSquared >= radiusSumSquared)
			{
				continue;
			}

			rigid->collision.other = rigidbody;

			if (rigid->collision.isTrigger || rigidbody->collision.isTrigger || rigidbody->isFrozen)
			{
				return false;
			}

			// 处理碰撞后的结果
			const float distance = sqrt(distanceSquared);
			const float dr = sqrt(radiusSumSquared) - distance;
			const float dx = dr * deltaX / distance;
			const float dy = dr * deltaY / distance;

			rigid->newPosition.x += dx;
			rigid->newPosition.y += dy;
			
			rigid->ApplyCollision();

			rigidVelocity.x = 0;
			rigidVelocity.y = 0;

			collisionOccurred = true;
		}

		return collisionOccurred;
	}

	bool BoundingBoxIntersect(Rigidbody* a, Rigidbody* b)
	{
		// 实现包围盒相交检测逻辑，如果相交返回true，否则返回false
		// 你可以根据刚体的形状和位置来实现具体的检测方法
		// 这里简单示范一个矩形包围盒的检测
		return (a->newPosition.x - a->collision.radius < b->newPosition.x + b->collision.radius) &&
			(a->newPosition.x + a->collision.radius > b->newPosition.x - b->collision.radius) &&
			(a->newPosition.y - a->collision.radius < b->newPosition.y + b->collision.radius) &&
			(a->newPosition.y + a->collision.radius > b->newPosition.y - b->collision.radius);
	}

	bool IsColliding(Rigidbody* body1, Rigidbody* body2)
	{
		const float deltaX = body1->actor->transform.position.x - body2->actor->transform.position.x;
		const float deltaY = body1->actor->transform.position.y - body2->actor->transform.position.y;
		const float distanceSquared = deltaX * deltaX + deltaY * deltaY;
		const float radiusSumSquared = (body1->collision.radius + body2->collision.radius) * (body1->collision.radius + body2->collision.radius);

		// 如果两个球型包围盒的距离的平方小于半径之和的平方，则它们相交
		return distanceSquared < radiusSumSquared;
	}

	QuadTreeNode* CreateQuadTreeNode(float minX, float minY, float maxX, float maxY)
	{
		QuadTreeNode* node = new QuadTreeNode;
		node->minX = minX;
		node->minY = minY;
		node->maxX = maxX;
		node->maxY = maxY;
		for (int i = 0; i < 4; i++) {
			node->children[i] = nullptr;
		}
		return node;
	}

	void PerformCollisionDetection(QuadTreeNode* node)
	{

		if (node == nullptr) 
		{
			return;
		}

		// 检测当前节点内的刚体碰撞
		for (size_t i = 0; i < node->rigidbodies.size(); i++) {
			for (size_t j = i + 1; j < node->rigidbodies.size(); j++) {

				if (node->rigidbodies[i]->isRemoved||node->rigidbodies[j]->isRemoved)
				{
					continue;
				}

				// 检测刚体是否碰撞，执行碰撞处理逻辑
				if (IsColliding(node->rigidbodies[i], node->rigidbodies[j])) {
					// 处理碰撞
					// 这里可以添加碰撞响应逻辑
					
					node->rigidbodies[i]->collision.other = node->rigidbodies[j];
					node->rigidbodies[j]->collision.other = node->rigidbodies[i];

					if (node->rigidbodies[i]->collision.isTrigger || node->rigidbodies[j]->collision.isTrigger || node->rigidbodies[j]->isFrozen)
					{
						continue;
					}

					// 检测两个刚体是否会相撞（使用平方距离）
					const float deltaX = node->rigidbodies[i]->actor->transform.position.x - node->rigidbodies[j]->actor->transform.position.x;
					const float deltaY = node->rigidbodies[i]->actor->transform.position.y - node->rigidbodies[j]->actor->transform.position.y;
					const float distanceSquared = deltaX * deltaX + deltaY * deltaY;

					const float radiusSumSquared = (node->rigidbodies[i]->collision.radius + node->rigidbodies[j]->collision.radius) * (node->rigidbodies[i]->collision.radius + node->rigidbodies[j]->collision.radius);

					// 处理碰撞后的结果
					const float distance = sqrt(distanceSquared);
					const float dr = sqrt(radiusSumSquared) - distance;
					const float dx = dr * deltaX / distance;
					const float dy = dr * deltaY / distance;

					node->rigidbodies[i]->newPosition.x += dx;
					node->rigidbodies[i]->newPosition.y += dy;

					node->rigidbodies[j]->newPosition.x -= dx;
					node->rigidbodies[j]->newPosition.y -= dy;

					node->rigidbodies[i]->velocity = { 0,0 };
					node->rigidbodies[j]->velocity = { 0,0 };

					node->rigidbodies[i]->UpdatePosition();
					node->rigidbodies[j]->UpdatePosition();

				}
			}
		}
	}

	void InsertRigidbody(QuadTreeNode* node,Rigidbody * rigidbody)
	{
		if (node == nullptr) 
		{
			return;
		}
		// 如果节点为空或者节点不包含子节点，则插入刚体
		if (node->children[0] == nullptr) {
			node->rigidbodies.push_back(rigidbody);
		}
		else {
			// 否则，将刚体插入到合适的子节点中
			float midX = (node->minX + node->maxX) / 2;
			float midY = (node->minY + node->maxY) / 2;
			int quadrant = (rigidbody->actor->transform.position.x < midX ? 0 : 1) + (rigidbody->actor->transform.position.y < midY ? 0 : 2);

			if (node->children[quadrant] == nullptr) {
				// 如果子节点不存在，则创建子节点
				float minX = quadrant % 2 == 0 ? node->minX : midX;
				float minY = quadrant < 2 ? node->minY : midY;
				float maxX = quadrant % 2 == 0 ? midX : node->maxX;
				float maxY = quadrant < 2 ? midY : node->maxY;
				node->children[quadrant] = CreateQuadTreeNode(minX, minY, maxX, maxY);
			}

			InsertRigidbody(node->children[quadrant], rigidbody);
		}
	}

}

