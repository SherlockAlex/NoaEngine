#pragma once

#include "GameEngine.h"
#include "LiveEntity.h"

using namespace noa;

class Enimy :public GameObject,public LiveEntity
{
public:
	Rigidbody* rigid = Rigidbody::Create(this);
	Transform* player = nullptr;
	StateMachine* fsm = nullptr;
	NoaEvent<void()> deathEvent;
	Animation* currentAnimation = nullptr;
	LiveEntity* enimy = nullptr;
	//int hp = 100;

protected:
	//void TakeDamage(int damage);
	Enimy(Scene * scene,Sprite* sprite, Transform* player,LiveEntity * enimy);
	~Enimy();
public:
	
	void Update() override;
	void OnDestroy() override;
	virtual void OnPain() = 0;
};

