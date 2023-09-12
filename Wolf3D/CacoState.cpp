#include "CacoState.h"
#include "Enimy.h"

#pragma region CacoIdleState

CacoIdleState::CacoIdleState(
	StateMachine* stateMachine,
	Enimy* enimy, Transform* target,
	const char* animationPath) :
	State(stateMachine)
{
	animation.LoadFromAnimationFile(animationPath);
	this->enimy = enimy;
	this->target = target;
}

void CacoIdleState::Act()
{
	animation.Play();
	enimy->currentAnimation = &animation;
	enimy->useMotion = false;
	enimy->velocity = { 0,0 };
}

void CacoIdleState::Reason()
{
	Vector<float> distanceVector = target->position - enimy->transform.position;
	const float distance = distanceVector.SqrMagnitude();

	if (distance <= 7 * 7)
	{
		enimy->useMotion = true;
		SetTransition(Move);
	}
	if (enimy->hp<=0) 
	{
		Debug("Enimy die");
		enimy->useMotion = false;
		enimy->velocity = { 0,0 };
		SetTransition(Die);
	}

}

#pragma endregion


#pragma region CacoMoveState

CacoMoveState::CacoMoveState(
	StateMachine* stateMachine,
	Enimy* enimy, Transform* target,
	const char* animationPath) :
	State(stateMachine)
{
	animation.LoadFromAnimationFile(animationPath);
	this->enimy = enimy;
	this->target = target;
}

void CacoMoveState::Act()
{
	animation.Play();
	enimy->currentAnimation = &animation;
	enimy->useMotion = true;
	Vector<float> distanceVector = target->position - enimy->transform.position;
	const float distance = distanceVector.SqrMagnitude();
	enimy->velocity = distanceVector.Normalize() * 2;
}

void CacoMoveState::Reason()
{
	Vector<float> distanceVector = target->position - enimy->transform.position;
	const float distance = distanceVector.SqrMagnitude();

	if (distance <= 2 * 2)
	{
		enimy->useMotion = false;
		enimy->velocity = { 0,0 };
		SetTransition(Attack);
	}
	if (enimy->hp <= 0)
	{
		Debug("Enimy die");
		enimy->useMotion = false;
		enimy->velocity = { 0,0 };
		SetTransition(Die);
	}

}

#pragma endregion

#pragma region CacoAttackState

CacoAttackState::CacoAttackState(
	StateMachine* stateMachine,
	Enimy* enimy, Transform* target,
	const char* animationPath) :
	State(stateMachine)
{
	animation.LoadFromAnimationFile(animationPath);
	this->enimy = enimy;
	this->target = target;

	animation.SetFrameEvent(2, [this]() 
		{
			Vector<float> distanceVector = this->target->position - this->enimy->transform.position;
			const float distance = distanceVector.SqrMagnitude();

			if (distance <= 2 * 2)
			{
				this->enimy->enimy->TakeDamage(10);
			}
			
		});

}

void CacoAttackState::Act()
{
	animation.Play();
	enimy->currentAnimation = &animation;
	enimy->useMotion = false;
	enimy->velocity = {0,0};
	//攻击完一次就返回Idle
}

void CacoAttackState::Reason()
{
	Vector<float> distanceVector = target->position - enimy->transform.position;
	const float distance = distanceVector.SqrMagnitude();

	if (distance > 2 * 2)
	{
		//enimy->useMotion = true;
		SetTransition(Move);
	}
	if (enimy->hp <= 0)
	{
		Debug("Enimy die");
		enimy->useMotion = false;
		enimy->velocity = { 0,0 };
		SetTransition(Die);
	}

}

#pragma endregion

#pragma region CacoDieState

CacoDieState::CacoDieState(
	StateMachine* stateMachine,
	Enimy* enimy, Transform* target,
	const char* animationPath):
	State(stateMachine)
{
	animation.LoadFromAnimationFile(animationPath);
	this->enimy = enimy;
	this->target = target;
	animation.SetFrameEvent(8, [this]()
		{
			this->enimy->isFrozen = true;
			this->enimy->Destroy();
			this->enimy->RemoveRigidbody();
		});
}

void CacoDieState::Act()
{
	animation.Play();
	enimy->currentAnimation = &animation;
	enimy->useMotion = false;
	enimy->velocity = { 0,0 };
	enimy->transform.posZ = 0.5 * enimy->transform.posZ;
	//播放完就
	//enimy->Destroy();

}

void CacoDieState::Reason()
{

}

#pragma endregion

