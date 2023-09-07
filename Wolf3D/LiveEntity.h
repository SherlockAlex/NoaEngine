#pragma once
#include "GameEngine.h"

using namespace noa;

class LiveEntity
{
public:
	int hp = 100;
	int maxHp = 100;

	NoaEvent<void()> death;

	virtual void TakeDamage(int damage);

};

