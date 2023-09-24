#pragma once
#include "GameEngine.h"
using namespace noa;
class Bullet:public GameObject
{
public:
	Rigidbody* rigid = Rigidbody::Create(this);
	Animation* animation = Animation::Create(this, 20, false);
	Vector<float> dir = {0,0};
	float speed = 9;
	

protected:
	Bullet(Scene* scene, Sprite* sprite);
public:
	static Bullet * Create(Scene* scene, Sprite* sprite);
	void Delete() override;

	~Bullet();

	void Update() override;

	void OnHitTile() override;

	void OnTrigger(Collision other) override;

};

