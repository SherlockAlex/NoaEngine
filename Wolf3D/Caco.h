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
	NoaEvent<void()> itemPickEvent;
private:

	Caco(Scene * scene,Sprite* sprite, Transform* player,LiveEntity * enimy);
	~Caco();
public:
	static Caco* Create(Scene* scene, Sprite* sprite, Transform* player, LiveEntity* enimy);
	void Delete() override;

	void OnPain() override;
	void OnDeath();

	void OnDestroy() override;

};

