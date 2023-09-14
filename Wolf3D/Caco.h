#pragma once
#include "Enimy.h"
#include "GameEngine.h"
class Caco :public Enimy
{
public:
	Sprite* item = nullptr;
	NoaEvent<void()> itemPickEvent;

	Caco(Sprite* sprite, Transform* player,LiveEntity * enimy);
	void OnPain() override;
	void OnDeath();

};

