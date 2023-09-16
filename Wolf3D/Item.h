#pragma once
#include "GameEngine.h"
using namespace noa;
class Item :public GameObject, public Rigidbody
{
public:
	NoaEvent<void()> pickEvent;

	Item(Sprite* sprite);
	void OnTrigger(Collision other) override;
	void Update() override;
	void OnDisable() override;
};

