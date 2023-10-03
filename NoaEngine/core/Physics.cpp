#include "Physics.h"
#include "NoaEngine.h"
#include "Scene.h"
#include "Actor.h"

#include <unordered_map>
#include <thread>
#include <stack>

using namespace std;

namespace noa 
{
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
			velocity.y += gravityWeight * gravityAcceration * Time::deltaTime;
		}

		if (useMotion)
		{
			//处理力和速度的关系，同时计算下一帧的位置信息
			velocity = (velocity * (1 - damping)) + (sumForce * (Time::deltaTime * invMass));
			newPosition = (this->actor->transform.position) + (velocity * Time::deltaTime);
			
			ApplyCollision();
		}

	}

	void Rigidbody::LateUpdate()
	{
		if (!active)
		{
			return;
		}
		ApplyCollision();
		if (collision.isTrigger && collision.other != nullptr && this->actor != nullptr)
		{
			this->actor->OnTrigger(collision);
		}
		collision.other = nullptr;

		if (collision.isHitCollisionTile && this->actor != nullptr)
		{
			this->actor->OnHitTile();
		}
		collision.isHitCollisionTile = false;

		//更新位置
		if (useMotion && (!isFrozen))
		{
			collision.isGrounded = false;
			collision.hitTileID = -1;
			this->actor->transform.position = newPosition;
		}
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
			|| tileMap->IsCollisionTile(newPosition.x + 0.999 + scaleX, newPosition.y + 0.999 + scaleY)
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

}

