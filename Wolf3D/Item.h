#pragma once
#include "GameEngine.h"
using namespace noa;
class Item :public GameObject
{
public:
	NoaEvent<void()> pickEvent;

	Rigidbody* rigid = new Rigidbody(this);

	Item(Scene * scene,Sprite* sprite);
	void OnTrigger(Collision other) override;
	void Update() override;
	void OnDisable() override;
};

