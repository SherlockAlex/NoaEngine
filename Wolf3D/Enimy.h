#pragma once

#include "GameEngine.h"
#include "LiveEntity.h"

using namespace noa;

class Enimy :public GameObject, public Rigidbody,public LiveEntity
{
public:
	Transform* player = nullptr;
	StateMachine* fsm = nullptr;
	NoaEvent<void()> deathEvent;
	Animator* currentAnimation = nullptr;
	LiveEntity* enimy = nullptr;
	//int hp = 100;

public:

	//void TakeDamage(int damage);
	Enimy(Sprite sprite, Transform* player,LiveEntity * enimy);
	~Enimy();
	void Update() override;

};

