#pragma once
#include "GameEngine.h"
using namespace noa;
class Item :public GameObject
{
public:
	NoaEvent<void()> pickEvent;

	Rigidbody* rigid = Rigidbody::Create(this);
private:
	Item(Scene * scene,Sprite* sprite);
	~Item();
public:
	static Item* Create(Scene* scene, Sprite* sprite);
	void Delete() override;

	void OnTrigger(Collision other) override;
	void Update() override;
	void OnDisable() override;
};

