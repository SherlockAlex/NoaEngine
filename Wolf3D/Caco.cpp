#include "Caco.h"
#include "CacoState.h"
#include "Player.h"
#include "Item.h"

Enimy* attackingEnimy = nullptr;

Caco::Caco(Sprite * sprite, Transform* player,LiveEntity * enimy) :Enimy(sprite, player,enimy)
{
	if (player == nullptr||fsm == nullptr) 
	{
		Debug("Get player error");
		exit(-1);
	}
	
	this->isRaycasted = true;

	this->collision.radius = 1;

	CacoIdleState* idleState = new CacoIdleState(fsm, this, player, "./Assets/Wolf/soldier-idle.amt");
	CacoMoveState* moveState = new CacoMoveState(fsm, this, player, "./Assets/Wolf/soldier-walk.amt");
	CacoAttackState* attackState = new CacoAttackState(fsm, this, player, "./Assets/Wolf/soldier-attack.amt");
	CacoDieState* dieState = new CacoDieState(fsm, this, player, "./Assets/Wolf/soldier-die.amt");
	CacoPainState* painState = new CacoPainState(fsm, this, player, "./Assets/Wolf/soldier-pain.amt");


	idleState->AddTransition(Move, moveState);
	idleState->AddTransition(Die, dieState);
	idleState->AddTransition(Pain, painState);

	moveState->AddTransition(Idle, idleState);
	moveState->AddTransition(Attack, attackState);
	moveState->AddTransition(Die, dieState);
	moveState->AddTransition(Pain, painState);

	attackState->AddTransition(Move, idleState);
	attackState->AddTransition(Die, dieState);
	attackState->AddTransition(Pain, painState);

	painState->AddTransition(Pain, painState);
	painState->AddTransition(Die, dieState);
	painState->AddTransition(Idle, idleState);

	fsm->AddState(idleState);
	fsm->AddState(painState);
	fsm->AddState(dieState);
	fsm->AddState(moveState);
	fsm->AddState(attackState);

}

void Caco::OnPain()
{
	if (fsm == nullptr||fsm->currentState == nullptr) 
	{
		return;
	}
	this->fsm->currentState->SetTransition(Pain);
}

void Caco::OnDeath()
{
	Item* bullet = new Item(item);
	bullet->transform.position = transform.position;
	bullet->transform.posZ = -250;
	bullet->pickEvent = this->itemPickEvent;
}

