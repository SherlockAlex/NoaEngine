#pragma once
#include "GameEngine.h"
class LiveEntity : public noa::Actor
{
protected:
	int hp = 100;
protected:
	LiveEntity(noa::Scene * scene);
	virtual ~LiveEntity();
public:
	virtual void TakeDamage(int damage) = 0;
};

