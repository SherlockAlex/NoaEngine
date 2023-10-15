#pragma once

#include "GameEngine.h"
#include "LiveEntity.h"

using namespace noa;

class Enimy :public Actor,public LiveEntity
{
private:
	Sprite* sprite = nullptr;
public:
	SpriteRenderer* spriteRenderer = SpriteRenderer::Create(this);
	Rigidbody* rigid = Rigidbody::Create(this);
	CircleCollider2D* collider = CircleCollider2D::Create(this, rigid);
	Transform* player = nullptr;
	StateMachine* fsm = StateMachine::Create(this);
	NoaEvent<void> deathEvent;
	Animation* currentAnimation = nullptr;
	LiveEntity* enimy = nullptr;

protected:
	Enimy(Scene * scene,Sprite* sprite, Transform* player,LiveEntity * enimy);
	virtual ~Enimy();
public:
	
	void Update() override;
	void OnDestroy() override;
	virtual void OnPain() = 0;
};

