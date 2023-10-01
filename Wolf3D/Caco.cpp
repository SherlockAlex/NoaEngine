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

	idleState = CacoIdleState::Create(fsm, this, player, &wolfResource->cacoIdleFrame);
	moveState = CacoMoveState::Create(fsm, this, player, &wolfResource->cacoMoveFrame);
	attackState = CacoAttackState::Create(fsm, this, player, &wolfResource->cacoAttckFrame);
	dieState = CacoDieState::Create(fsm, this, player, &wolfResource->cacoDieFrame);
	painState = CacoPainState::Create(fsm, this, player, &wolfResource->cacoPainFrame);


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

	fsm->AddState(idleState);
	fsm->AddState(painState);
	fsm->AddState(dieState);
	fsm->AddState(moveState);
	fsm->AddState(attackState);

}

Caco::~Caco()
{
	//Enimy::~Enimy();
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
	if (fsm->currentState == nullptr) 
	{
		return;
	}
	this->fsm->PerformTransition(Pain);
	//this->fsm->currentState->SetTransition(Pain);
}

void Caco::OnDeath()
{
	Item* bullet = Item::Create(this->activeScene,item);
	bullet->transform.position = transform.position;
	bullet->transform.posZ = -250;
	bullet->pickEvent = this->itemPickEvent;
}

void Caco::Awake()
{
	
}

void Caco::OnDestroy()
{
	Enimy::OnDestroy();
}

