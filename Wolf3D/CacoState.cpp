#include "CacoState.h"
#include "Enimy.h"
#include "Caco.h"
#include "Bullet.h"
#include "WolfResource.h"

#pragma region CacoIdleState

extern Enimy* attackingEnimy;

CacoIdleState::CacoIdleState(
	StateMachine* stateMachine,
	Enimy* enimy, Transform* target,
	AnimationFrame* frameData
) :
	State(stateMachine)
{
	animation.SetFrame(frameData);
	animation.SetActiveScene(enimy->GetActiveScene());
	//animation.LoadFromAnimationFile(animationPath);
	this->enimy = enimy;
	this->target = target;
}

void CacoIdleState::OnUpdate()
{
	animation.Play();
	enimy->currentAnimation = &animation;
	enimy->rigid->useMotion = false;
	enimy->rigid->velocity = { 0,0 };
	if (attackingEnimy == this->enimy)
	{
		attackingEnimy = nullptr;
	}
}

void CacoIdleState::Reason()
{
	Vector<float> distanceVector = target->position - enimy->transform.position;
	const float distance = distanceVector.SqrMagnitude();

	if (distance <= 5 * 5)
	{
		enimy->rigid->useMotion = true;
		animation.Reset();
		SetTransition(Move);
	}
	if (enimy->hp<=0) 
	{
		Debug("Enimy die");
		enimy->rigid->useMotion = false;
		enimy->rigid->velocity = { 0,0 };
		animation.Reset();
		SetTransition(Die);
	}

}

#pragma endregion


#pragma region CacoMoveState

CacoMoveState::CacoMoveState(
	StateMachine* stateMachine,
	Enimy* enimy, Transform* target,
	AnimationFrame* frameData
) :
	State(stateMachine)
{
	animation.SetFrame(frameData);
	animation.SetActiveScene(enimy->GetActiveScene());
	//animation.LoadFromAnimationFile(animationPath);
	this->enimy = enimy;
	this->target = target;
}

void CacoMoveState::OnUpdate()
{
	if (attackingEnimy == this->enimy)
	{
		attackingEnimy = nullptr;
	}
	animation.Play();
	enimy->currentAnimation = &animation;
	enimy->rigid->useMotion = true;
	Vector<float> distanceVector = target->position - enimy->transform.position;
	const float distance = distanceVector.SqrMagnitude();
	enimy->rigid->velocity = distanceVector.Normalize() * 2;
}

void CacoMoveState::Reason()
{
	Vector<float> distanceVector = target->position - enimy->transform.position;
	const float distance = distanceVector.SqrMagnitude();

	if (distance <= 3 * 3)
	{
		enimy->rigid->useMotion = false;
		enimy->rigid->velocity = { 0,0 };
		if (attackingEnimy == nullptr) {
			animation.Reset();
			SetTransition(Attack);
		}
		else {
			return;
		}
		
	}
	if (enimy->hp <= 0)
	{
		Debug("Enimy die");
		enimy->rigid->useMotion = false;
		enimy->rigid->velocity = { 0,0 };
		animation.Reset();
		SetTransition(Die);
	}

}

#pragma endregion

#pragma region CacoAttackState

CacoAttackState::CacoAttackState(
	StateMachine* stateMachine,
	Enimy* enimy, Transform* target,
	AnimationFrame* frameData
) :
	State(stateMachine)
{
	animation.SetFrame(frameData);
	animation.SetActiveScene(enimy->GetActiveScene());
	//animation.LoadFromAnimationFile(animationPath);
	this->enimy = enimy;
	this->target = target;

	animation.SetFrameEvent(1, [this]() 
		{
			Vector<float> distanceVector = this->target->position - this->enimy->transform.position;
			const float distance = distanceVector.SqrMagnitude();

			if (distance <= 3 * 3)
			{
				//this->enimy->enimy->TakeDamage(2);
				//实现角色的raycasting，如果集中的是墙壁
				Bullet* bullet = new Bullet(this->enimy->GetActiveScene(), &wolfResource.firebomb);
				bullet->rigid->SetTileMap(this->enimy->rigid->GetTileMap());
				bullet->transform.position = this->enimy->transform.position;
				bullet->dir = distanceVector.Normalize();
				audio.Play(false);
			}
			
		});

}

void CacoAttackState::OnUpdate()
{
	attackingEnimy = this->enimy;
	animation.Play();
	enimy->currentAnimation = &animation;
	enimy->rigid->useMotion = false;
	enimy->rigid->velocity = {0,0};
	//攻击完一次就返回Idle
}

void CacoAttackState::Reason()
{
	Vector<float> distanceVector = target->position - enimy->transform.position;
	const float distance = distanceVector.SqrMagnitude();

	if (distance > 3 * 3)
	{
		animation.Reset();
		SetTransition(Move);
	}
	if (enimy->hp <= 0)
	{
		Debug("Enimy die");
		enimy->rigid->useMotion = false;
		enimy->rigid->velocity = { 0,0 };
		animation.Reset();
		SetTransition(Die);
	}

}

#pragma endregion

#pragma region CacoDieState

CacoDieState::CacoDieState(
	StateMachine* stateMachine,
	Enimy* enimy, Transform* target,
	AnimationFrame* frameData
):
	State(stateMachine)
{
	animation.SetFrame(frameData);
	animation.SetActiveScene(enimy->GetActiveScene());
	//animation.LoadFromAnimationFile(animationPath);
	this->enimy = enimy;
	this->target = target;

	animation.SetFrameEvent(1, [this]() {
		audio.Play(false);
		this->enimy->rigid->isFrozen = true;
		((Caco*)this->enimy)->OnDeath();
	});

	animation.SetFrameEvent(10, [this]()
		{
			
			this->enimy->Destroy();
			this->enimy->rigid->Destroy();
			this->enimy->deathEvent.Invoke();
		});
}

void CacoDieState::OnEnter()
{
	//((Caco*)enimy)->OnDeath();
}

void CacoDieState::OnUpdate()
{
	if (attackingEnimy == this->enimy)
	{
		attackingEnimy = nullptr;
	}
	animation.Play();
	enimy->currentAnimation = &animation;
	enimy->rigid->useMotion = false;
	enimy->rigid->velocity = { 0,0 };
	enimy->transform.posZ = 0.5 * enimy->transform.posZ;

}

void CacoDieState::Reason()
{

}

#pragma endregion

#pragma region CacoPainState

CacoPainState::CacoPainState(
	StateMachine* stateMachine,
	Enimy* enimy, Transform* target,
	AnimationFrame* frameData
) :
	State(stateMachine)
{
	animation.SetFrame(frameData);
	animation.SetActiveScene(enimy->GetActiveScene());
	this->enimy = enimy;
	this->target = target;

	animation.SetFrameEvent(1, [this]() {
		audio.Play(false);
	});

	animation.SetFrameEvent(1, [this]()
		{
			if (this->enimy->hp<=0)
			{
				SetTransition(Die);
			}
			SetTransition(Idle);
		});
}

void CacoPainState::OnUpdate()
{
	if (attackingEnimy == this->enimy)
	{
		attackingEnimy = nullptr;
	}
	animation.Play();
	enimy->currentAnimation = &animation;
	enimy->rigid->useMotion = false;
	enimy->rigid->velocity = { 0,0 };
	enimy->transform.posZ = 0.5 * enimy->transform.posZ;

}

void CacoPainState::Reason()
{
	if (this->enimy->hp <= 0)
	{
		animation.Reset();
		SetTransition(Die);
	}
}

#pragma endregion
