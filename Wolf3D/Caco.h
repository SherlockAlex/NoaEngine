#pragma once
#include "Enimy.h"
#include "GameEngine.h"
class Caco final:public Enimy
{
public:
	Sprite* item = nullptr;
	NoaEvent<void()> itemPickEvent;
private:

	Caco(Scene * scene,Sprite* sprite, Transform* player,LiveEntity * enimy);
public:
	static Caco* Create(Scene* scene, Sprite* sprite, Transform* player, LiveEntity* enimy);

	void OnPain() override;
	void OnDeath();

};

