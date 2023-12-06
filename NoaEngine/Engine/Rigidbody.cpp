#include "Actor.h"
#include "ActorComponent.h"
#include "Scene.h"
#include "Debug.h"
#include "Rigidbody.h"
#include "PhysicsSystem.h"

#include "Collider2D.h"
#include "BoxCollider2D.h"
#include "CircleCollider2D.h"
#include "TileCollider2D.h"

#include <unordered_map>
#include <thread>
#include <stack>

noa::Rigidbody::Rigidbody(Actor* actor) :ActorComponent(actor)
{
	this->transform = &actor->transform;
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
		colliders.clear();
	}

}

void noa::Rigidbody::Start()
{

}

void noa::Rigidbody::Update()
{
	if (!GetActive())
	{
		return;
	}

	//上传rigidbody到物理系统中
	PhysicsSystem::rigidbodys.push_back(this);
}

void noa::Rigidbody::InitVelocity(float deltaTime)
{
	if (this->bodyType == BodyType::STATIC)
	{
		return;
	}

	Vector<float> gravity = PhysicsSystem::gravity * this->gravityScale*this->mass;
	if (tileCollider2D != nullptr && tileCollider2D->isGrounded)
	{
		gravity = {};
	}
	
	if (useGravity)
	{
		AddForce(gravity, ForceType::CONTINUOUS_FORCE);
	}

	const float deltaVX = 
		force.x * invMass * deltaTime + impuls.x * invMass;
	const float deltaVY =
		force.y * invMass * deltaTime + impuls.y * invMass;

	/*if (std::abs(velocity.x) < 0.01f
		&& std::abs(velocity.y) < 0.01f)
	{
		velocity.x = 0.0f;
		velocity.y = 0.0f;
		sleep = true;
	}*/

	newVelocity.x = (velocity.x + deltaVX);
	newVelocity.y = (velocity.y + deltaVY);
	//计算角动量

	sleep = false;

	force = {};
	impuls = {};

	constraint = nextConstraint;
	nextConstraint.x = false;
	nextConstraint.y = false;

	//当两者的deltaVX和deltaVY都趋于0的时候，开启sleep
	//开启sleep后，速度后归0，同时位置不改变
	

}

void noa::Rigidbody::ApplyVelocity(float deltaTime)
{
	if (this->bodyType == BodyType::STATIC)
	{
		return;
	}

	if (this->newVelocity.x > 100.0f)
	{
		this->newVelocity.x = 100.0f;
	}
	else if(this->newVelocity.x < -100.0f){
		this->newVelocity.x = -100.0f;
	}


	if (this->newVelocity.y > 100.0f)
	{
		this->newVelocity.y = 100.0f;
	}
	else if (this->newVelocity.y < -100.0f) {
		this->newVelocity.y = -100.0f;
	}

	this->velocity = this->newVelocity;

}

void noa::Rigidbody::InitPosition(float deltaTime)
{
	newPosition.x = transform->position.x + velocity.x * deltaTime;
	newPosition.y = transform->position.y + velocity.y * deltaTime;
}

void noa::Rigidbody::ApplyPositon(float deltaTime)
{
	transform->position = newPosition;
	transform->eulerAngle = transform->eulerAngle + angularVelocity * angularVelocityWeight;
}

noa::Vector<float> noa::Rigidbody::GetSumForce()
{
	return this->force;
}

noa::Vector<float> noa::Rigidbody::GetMomentum()
{
	return this->velocity * this->mass;
}

void noa::Rigidbody::AddForce(const Vector<float>& force, ForceType forceType)
{

	if (this->bodyType!=BodyType::DYNAMIC) 
	{
		return;
	}

	switch (forceType)
	{
	case ForceType::CONTINUOUS_FORCE:
		this->force.x = this->force.x + force.x;
		this->force.y = this->force.y + force.y;
		break;
	case ForceType::IMPULSE_FORCE:
		this->impuls.x = this->impuls.x + force.x;
		this->impuls.y = this->impuls.y + force.y;
		break;
	default:
		break;
	}

}

noa::Rigidbody& noa::Rigidbody::SetMass(float value)
{
	this->mass = value;
	this->invMass = 1.0f / value;
	return *this;
}

noa::Rigidbody& noa::Rigidbody::SetBodyType(BodyType bodyType)
{
	this->bodyType = bodyType;
	return *this;
}

noa::Rigidbody& noa::Rigidbody::SetBounce(float value)
{
	this->bounce = value;
	return *this;
}

noa::Rigidbody& noa::Rigidbody::SetFriction(float value)
{
	this->friction = value;
	return *this;
}

noa::Rigidbody& noa::Rigidbody::SetDamping(float value)
{
	this->damping = value;
	return *this;
}

noa::Rigidbody& noa::Rigidbody::SetGravityScale(float value)
{
	this->gravityScale = value;
	return *this;
}

noa::Rigidbody& noa::Rigidbody::SetAngularScale(float value)
{
	this->angularVelocityWeight = value;
	return *this;
}

noa::Rigidbody& noa::Rigidbody::SetIsFrozen(bool value)
{
	this->isFrozen = value;
	return *this;
}

noa::Rigidbody& noa::Rigidbody::SetUseGravity(bool value)
{
	this->useGravity = value;
	return *this;
}

noa::Rigidbody* noa::Rigidbody::Apply()
{
	return this;
}

noa::Vector<bool> noa::Rigidbody::GetConstraint()
{
	return this->constraint;
}

void noa::Rigidbody::AddAntiGravity()
{
	this->AddForce(PhysicsSystem::gravity * gravityScale * this->mass*(-1.0f), ForceType::CONTINUOUS_FORCE);
}

void noa::Rigidbody::ApplyTileCollision(float deltaTime)
{

	if (!this->tileCollider2D)
	{
		return;
	}
	this->tileCollider2D->ApplyTileCollision(deltaTime);

}

void noa::Rigidbody::ApplyTileConstraint(float deltaTime)
{
	if (!this->tileCollider2D)
	{
		return;
	}
	this->tileCollider2D->ApplyConstraint(deltaTime);
}

void noa::Rigidbody::BindCollider(Collider2D* collider)
{
	colliders.push_back(collider);
}

noa::Rigidbody* noa::Rigidbody::Create(Actor* actor)
{
	return noa::NObject<Rigidbody>::Create(actor);
}

