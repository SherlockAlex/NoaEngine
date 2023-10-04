#pragma once
#include "Enimy.h"
#include "GameEngine.h"

class CacoIdleState;
class CacoMoveState;
class CacoAttackState;
class CacoDieState;
class CacoPainState;

class Caco final:public Enimy
{

public:
	Sprite* item = nullptr;
	NoaEvent<void> itemPickEvent;
private:

	Caco(Scene * scene,Sprite* sprite, Transform* player,LiveEntity * enimy);
	~Caco() override;
public:
	static Caco* Create(Scene* scene, Sprite* sprite, Transform* player, LiveEntity* enimy);
	void Delete() override;

	void OnPain() override;
	void OnDeath();

	void Awake() override;

	void OnDestroy() override;

public:

	CacoIdleState* idleState = nullptr;
	CacoMoveState* moveState = nullptr;
	CacoAttackState* attackState = nullptr;
	CacoDieState* dieState = nullptr;
	CacoPainState* painState = nullptr;

};

