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

	this->transform = &actor->transform;

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
	if (!GetActive())
	{
		return;
	}
	
	PhysicsSystem::rigidbodys.push_back(this);
}

void noa::Rigidbody::InitVelocity(float deltaTime)
{
	if (transform == nullptr || this->bodyType == BodyType::STATIC)
	{
		return;
	}

	if (useGravity)
	{
		Vector<float> gravity = PhysicsSystem::gravity * this->gravityWeight;
		if (tileCollider2D != nullptr && tileCollider2D->isGrounded)
		{
			gravity = {};
		}
		AddForce(gravity, ForceType::CONTINUOUS_FORCE);
	}

	this->newVelocity = (this->velocity + this->force * invMass * deltaTime + impuls * invMass);
	
	force = {};
	impuls = {};

	constraint.x = false;
	constraint.y = false;

}

void noa::Rigidbody::ApplyVelocity(float deltaTime)
{
	if (transform == nullptr || this->bodyType == BodyType::STATIC)
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

void noa::Rigidbody::InitAngleVelocity(float deltaTime)
{
	if (this->bodyType==BodyType::STATIC) 
	{
		return;
	}
	// 角速度 = 角加速度 + 角动量*invMath
	this->newAngleVelocity = this->angleVelocity + 0;
}

void noa::Rigidbody::InitPosition(float deltaTime)
{
	if (!this->transform) {
		return;
	}
	this->newPosition = this->transform->position + this->velocity * deltaTime;
	
}

void noa::Rigidbody::ApplyPositon(float deltaTime)
{
	if (!this->transform) 
	{
		return;
	}

	transform->position = newPosition;
	
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
		this->force = this->force + force;
		break;
	case ForceType::IMPULSE_FORCE:
		this->impuls = this->impuls + force;
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

void noa::Rigidbody::SetBodyType(BodyType bodyType)
{
	this->bodyType = bodyType;
}

noa::Vector<bool> noa::Rigidbody::GetConstraint()
{
	return this->constraint;
}

void noa::Rigidbody::AddAntiGravity()
{
	this->AddForce(PhysicsSystem::gravity * gravityWeight * this->mass*(-1.0f), ForceType::CONTINUOUS_FORCE);
}

void noa::Rigidbody::ApplyTileCollision(float deltaTime)
{

	if (this->tileCollider2D == nullptr)
	{
		return;
	}
	this->tileCollider2D->ApplyTileCollision(deltaTime);

}

void noa::Rigidbody::ApplyTileConstraint(float deltaTime)
{
	if (this->tileCollider2D == nullptr)
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

bool noa::Collision::CompareTag(const std::string& tag) const
{
	if (actor == nullptr)
	{
		return false;
	}
	return actor->tag == tag;
}


