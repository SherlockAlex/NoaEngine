#include "Caco.h"
#include "CacoState.h"
#include "Player.h"
#include "Item.h"
#include "WolfResource.h"

Enimy* attackingEnimy = nullptr;

//CacoIdleState* idleState;
//CacoMoveState* moveState;
//CacoAttackState* attackState;
//CacoDieState* dieState;
//CacoPainState* painState;

Caco::Caco(Scene* scene, Sprite* sprite, Transform* player, LiveEntity* enimy) :Enimy(scene,sprite, player,enimy)
{
	if (player == nullptr) 
	{
		Debug("Get player error");
		exit(-1);
	}
	
	this->isRaycasted = true;

	this->rigid->collision.radius = 1;

	/*CacoIdleState* idleState = CacoIdleState::Create(fsm.get(), this, player, &wolfResource->cacoIdleFrame);
	CacoMoveState* moveState = CacoMoveState::Create(fsm.get(), this, player, &wolfResource->cacoMoveFrame);
	CacoAttackState* attackState = CacoAttackState::Create(fsm.get(), this, player, &wolfResource->cacoAttckFrame);
	CacoDieState* dieState = CacoDieState::Create(fsm.get(), this, player, &wolfResource->cacoDieFrame);
	CacoPainState* painState = CacoPainState::Create(fsm.get(), this, player, &wolfResource->cacoPainFrame);*/

	/*idleState = make_shared<CacoIdleState>(&fsm, this, player, &wolfResource->cacoIdleFrame);
	moveState = make_shared<CacoMoveState>(&fsm, this, player, &wolfResource->cacoMoveFrame);
	attackState = make_shared<CacoAttackState>(&fsm, this, player, &wolfResource->cacoAttckFrame);
	dieState = make_shared<CacoDieState>(&fsm, this, player, &wolfResource->cacoDieFrame);
	painState = make_shared<CacoPainState>(&fsm, this, player, &wolfResource->cacoPainFrame);*/

	idleState = CacoIdleState::Create(&fsm, this, player, &wolfResource->cacoIdleFrame);
	moveState = CacoMoveState::Create(&fsm, this, player, &wolfResource->cacoMoveFrame);
	attackState = CacoAttackState::Create(&fsm, this, player, &wolfResource->cacoAttckFrame);
	dieState = CacoDieState::Create(&fsm, this, player, &wolfResource->cacoDieFrame);
	painState = CacoPainState::Create(&fsm, this, player, &wolfResource->cacoPainFrame);

	/*idleState = CacoIdleState(&fsm, this, player, &wolfResource->cacoIdleFrame);
	moveState = CacoMoveState(&fsm, this, player, &wolfResource->cacoMoveFrame);
	attackState = CacoAttackState(&fsm, this, player, &wolfResource->cacoAttckFrame);
	dieState = CacoDieState(&fsm, this, player, &wolfResource->cacoDieFrame);
	painState = CacoPainState(&fsm, this, player, &wolfResource->cacoPainFrame);*/

	/*if (idleState == nullptr)
	{
		idleState = new CacoIdleState(fsm, this, player, &wolfResource->cacoIdleFrame);
	}
	
	if (moveState == nullptr) 
	{
		moveState = new CacoMoveState(fsm, this, player, &wolfResource->cacoMoveFrame);
	}

	if (attackState == nullptr)
	{
		attackState = new CacoAttackState(fsm, this, player, &wolfResource->cacoAttckFrame);
	}

	if (dieState == nullptr)
	{
		dieState = new CacoDieState(fsm, this, player, &wolfResource->cacoDieFrame);
	}
	
	if (painState == nullptr)
	{
		painState = new CacoPainState(fsm, this, player, &wolfResource->cacoPainFrame);
	}*/


	idleState->AddTransition(Move,moveState);
	idleState->AddTransition(Die,dieState);
	idleState->AddTransition(Pain,painState);

	moveState->AddTransition(Idle,idleState);
	moveState->AddTransition(Attack,attackState);
	moveState->AddTransition(Die,dieState);
	moveState->AddTransition(Pain,painState);

	attackState->AddTransition(Move,idleState);
	attackState->AddTransition(Die,dieState);
	attackState->AddTransition(Pain,painState);

	painState->AddTransition(Pain,painState);
	painState->AddTransition(Die,dieState);
	painState->AddTransition(Idle,idleState);

	fsm.AddState(idleState);
	fsm.AddState(painState);
	fsm.AddState(dieState);
	fsm.AddState(moveState);
	fsm.AddState(attackState);

}

Caco::~Caco()
{
	Enimy::~Enimy();
	if (sprite != nullptr)
	{
		delete sprite;
	}
	Debug("Remove Caco");
}

Caco* Caco::Create(Scene* scene, Sprite* sprite, Transform* player, LiveEntity* enimy)
{
	return new Caco(scene, sprite, player, enimy);
}

void Caco::Delete()
{
	delete this;
}

void Caco::OnPain()
{
	if (fsm.currentState == nullptr) 
	{
		return;
	}
	this->fsm.currentState->SetTransition(Pain);
}

void Caco::OnDeath()
{
	Item* bullet = Item::Create(this->activeScene,item);
	bullet->transform.position = transform.position;
	bullet->transform.posZ = -250;
	bullet->pickEvent = this->itemPickEvent;
}

void Caco::OnDestroy()
{
	Enimy::OnDestroy();
}

