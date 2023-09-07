#include "Caco.h"
#include "CacoState.h"
#include "Player.h"


Caco::Caco(Sprite sprite, Transform* player,LiveEntity * enimy) :Enimy(sprite, player,enimy)
{
	if (player == nullptr||fsm == nullptr) 
	{
		Debug("Get player error");
		exit(0);
	}
	
	CacoIdleState* idleState = new CacoIdleState(fsm, this, player, "./Assets/Wolf/caco-idle.amt");
	CacoMoveState* moveState = new CacoMoveState(fsm, this, player, "./Assets/Wolf/caco-idle.amt");
	CacoAttackState* attackState = new CacoAttackState(fsm, this, player, "./Assets/Wolf/caco-attack.amt");
	CacoDieState* dieState = new CacoDieState(fsm, this, player, "./Assets/Wolf/caco-die.amt");

	idleState->AddTransition(Move, moveState);
	idleState->AddTransition(Die, dieState);
	moveState->AddTransition(Idle, idleState);
	moveState->AddTransition(Attack, attackState);
	moveState->AddTransition(Die, dieState);
	attackState->AddTransition(Move, idleState);
	attackState->AddTransition(Die, dieState);

	fsm->AddState(idleState);
	fsm->AddState(moveState);
	fsm->AddState(attackState);
	fsm->AddState(dieState);
}