#include "Actor.h"
#include "ActorComponent.h"
#include "Scene.h"
#include "Debug.h"
#include "Physics.h"
#include "PhysicsSystem.h"
#include "Collider2D.h"

#include <unordered_map>
#include <thread>
#include <stack>

using namespace std;

noa::Rigidbody::Rigidbody(Actor* actor) :ActorComponent(actor)
{

	collision.actor = nullptr;
	this->velocity = { 0,0 };
	invMass = 1.0f / mass;
	colliders.reserve(10);

}

noa::Rigidbody::~Rigidbody()
{
	if (!colliders.empty())
	{
		for (auto& collider : colliders)
		{
			collider->rigidbody = nullptr;
		}
	}

}

void noa::Rigidbody::Start()
{

}

void noa::Rigidbody::Update()
{
	PhysicsSystem::rigidbodys.push_back(this);
}

void noa::Rigidbody::UpdateVelocity(float deltaTime)
{

	if (!GetActive())
	{
		return;
	}

	const bool canApplyGravity =
		useMotion && useGravity && (tileCollider2D == nullptr || !tileCollider2D->isGrounded);
	if (canApplyGravity)
	{
		// 判断是否isGrounded
		this->force += (PhysicsSystem::gravity * gravityWeight);
	}

	if (useMotion)
	{
		velocity = (velocity * (1 - damping)) + (this->force * (deltaTime * invMass));
		newPosition = (GetActor()->transform.position) + (velocity * deltaTime);

	}

	this->ApplyTileCollision();

	this->force = {};

}

void noa::Rigidbody::UpdatePosition(float deltaTime)
{
	if (!GetActive())
	{
		return;
	}

	this->ApplyTileCollision();

	if (useMotion && (!isFrozen) && GetActor() != nullptr)
	{
		GetActor()->transform.position = newPosition;
	}
}

void noa::Rigidbody::AddForce(const Vector<float>& force, ForceType forceType)
{
	//添加力到物体上

	switch (forceType)
	{
	case ForceType::CONTINUOUS_FORCE:
		//添加恒力到物体上
		this->force = this->force + force;
		break;
	case ForceType::IMPULSE_FORCE:
		//添加一个冲量到物体上，作用完马上就消失
		velocity += force * invMass;
		break;
	default:
		break;
	}

}

void noa::Rigidbody::SetMass(float value)
{
	this->mass = value;
	this->invMass = 1.0f / value;
}

noa::Vector<float> pos(0.0, 0.0);
void noa::Rigidbody::ApplyTileCollision()
{

	if (this->tileCollider2D == nullptr)
	{
		return;
	}
	this->tileCollider2D->ApplyTileCollision();

}

void noa::Rigidbody::BindCollider(Collider2D* collider)
{
	colliders.push_back(collider);
}

noa::Rigidbody* noa::Rigidbody::Create(Actor* actor)
{
	return noa::NObject<Rigidbody>::Create(actor);
}

bool noa::Collision::CompareTag(const std::string& tag) const
{
	if (actor == nullptr)
	{
		return false;
	}
	return actor->tag == tag;
}


