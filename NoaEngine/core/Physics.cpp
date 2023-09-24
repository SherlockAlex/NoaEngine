#include "Physics.h"
#include "NoaEngine.h"
#include "Scene.h"
#include <unordered_map>
#include <thread>
#include "Actor.h"

using namespace std;

namespace noa 
{

	extern float deltaTime;
	//���rigid�Ƿ������������ײ�������ײ�ͷ���
	extern bool CollisionWithRigidbody(Rigidbody* rigid);
	extern bool BoundingBoxIntersect(Rigidbody* a, Rigidbody* b);
	float gravityAcceration = 9.81;
	
	unordered_map<size_t, bool> isCheckCollision;

	Rigidbody::Rigidbody(Actor * actor)
	{

		id = GetNextId();
		collision.other = nullptr;
		this->actor = actor;
		this->velocity = { 0,0 };
		invMass = 1.0 / mass;
		actor->GetActiveScene()->AddRigidbody(this);
		actor->AddRigidbody(this);
		//sceneManager.AddRigidbody(this);
	}

	// ��ʼ����̬������
	size_t Rigidbody::nextId = 0;

	Rigidbody::~Rigidbody()
	{
		Debug("Destory rigidBody");
		if (actor!=nullptr) 
		{
			actor->RemoveRigidbody(this);
		}
	}

	void Rigidbody::Start()
	{

	}

	//ʵ������Ч��
	void Rigidbody::Update()
	{
		if (!active) 
		{
			return;
		}
		
		const int x = this->actor->transform.position.x;
		const int y = this->actor->transform.position.y;
		indexInMap = (x << 16) | y;

		collision.other = nullptr;
		collision.isHitCollisionTile = false;

		/*if (deltaTime>1.0/10)
		{
			deltaTime = 1.0 / 60;
		}*/

		if (useMotion&&useGravity&&(!collision.isGrounded)) 
		{
			velocity.y += gravityWeight * gravityAcceration * deltaTime;
		}

		if (useMotion)
		{

			//���������ٶȵĹ�ϵ
			//F = ma
			velocity = (velocity * (1 - damping)) + (sumForce * (deltaTime * invMass));
			newPosition = (this->actor->transform.position) + (velocity * deltaTime);
			collision.isGrounded = false;
			collision.hitTileID = -1;
			ApplyCollision();
			//������ײ���
			CollisionWithinRigidbody();
			ApplyCollision();
			
			//colliderPos->position = newPosition;
		}

		if (useMotion&&(!isFrozen))
		{
			this->actor->transform.position = newPosition;
		}

		if (collision.isHitCollisionTile&& this->actor!=nullptr)
		{
			this->actor->OnHitTile();
			//OnHitTile();
		}
		collision.isHitCollisionTile = false;

		//�����ٶȽ����������ײ���
		//�����⵽����ײ�ַ�����ֹͣ
		if (collision.isTrigger && collision.other != nullptr && this->actor != nullptr)
		{
			this->actor->OnTrigger(collision);
			//OnTrigger(collision);
		}
		collision.other = nullptr;

		

	}

	void Rigidbody::AddForce(const Vector<float> force, ForceType forceType)
	{
		//�������������
		switch (forceType)
		{
		case Rigidbody::ContinuousForce:
			//��Ӻ�����������
			sumForce += force;
			break;
		case Rigidbody::Impulse:
			//���һ�������������ϣ����������Ͼ���ʧ
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
		//���������Ѿ������٣������Ǳ�����
		this->actor->GetActiveScene()->RemoveRigidbody(this);
	}

	int Rigidbody::GetIndexInMap() const
	{
		return this->indexInMap;
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
		if (!this->active|| this->actor->GetActiveScene() == nullptr)
		{
			return false;
		}

		const size_t hashCode = this->GetHashCode();
		const float rigidRadius = this->collision.radius;
		const float rigidX = this->newPosition.x;
		const float rigidY = this->newPosition.y;
		Vector<float>& rigidVelocity = this->velocity;

		bool collisionOccurred = false;

		for (const auto& e : this->actor->GetActiveScene()->rigidbodys)
		{
			if (e.first == hashCode|| e.second == nullptr||!e.second->GetActive())
			{
				continue;
			}

			Rigidbody* rigidbody = e.second;

			// ����Χ���Ƿ��ཻ
			if (!BoundingBoxIntersect(this, rigidbody))
			{
				continue;
			}

			// ������������Ƿ����ײ��ʹ��ƽ�����룩
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

			// ������ײ��Ľ��
			const float distance = sqrt(distanceSquared);
			const float dr = sqrt(radiusSumSquared) - distance;
			const float dx = distance == 0?0:(dr * deltaX / distance);
			const float dy = distance == 0?0:(dr * deltaY / distance);

			this->newPosition.x += dx;
			this->newPosition.y += dy;

			rigidVelocity.x = 0;
			rigidVelocity.y = 0;

			collisionOccurred = true;
		}

		return collisionOccurred;
	}

	/*void Rigidbody::RemoveRigidbody() const
	{
		DestroyRigidbody(this);
	}*/

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

			// ����Χ���Ƿ��ཻ
			if (!BoundingBoxIntersect(rigid, rigidbody))
			{
				continue;
			}

			// ������������Ƿ����ײ��ʹ��ƽ�����룩
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

			// ������ײ��Ľ��
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
		// ʵ�ְ�Χ���ཻ����߼�������ཻ����true�����򷵻�false
		// ����Ը��ݸ������״��λ����ʵ�־���ļ�ⷽ��
		// �����ʾ��һ�����ΰ�Χ�еļ��
		return (a->newPosition.x - a->collision.radius < b->newPosition.x + b->collision.radius) &&
			(a->newPosition.x + a->collision.radius > b->newPosition.x - b->collision.radius) &&
			(a->newPosition.y - a->collision.radius < b->newPosition.y + b->collision.radius) &&
			(a->newPosition.y + a->collision.radius > b->newPosition.y - b->collision.radius);
	}

}

