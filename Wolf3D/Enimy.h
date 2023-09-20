#pragma once

#include "GameEngine.h"
#include "LiveEntity.h"

using namespace noa;

class Enimy :public GameObject,public LiveEntity
{
public:
	Rigidbody* rigid = new Rigidbody(this);
	Transform* player = nullptr;
	StateMachine* fsm = nullptr;
	NoaEvent<void()> deathEvent;
	Animation* currentAnimation = nullptr;
	LiveEntity* enimy = nullptr;
	//int hp = 100;

public:

	//void TakeDamage(int damage);
	Enimy(Scene * scene,Sprite* sprite, Transform* player,LiveEntity * enimy);
	~Enimy();
	void Update() override;
	virtual void OnPain() = 0;
};

