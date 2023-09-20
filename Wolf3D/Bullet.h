#pragma once
#include "GameEngine.h"
using namespace noa;
class Bullet:public GameObject
{
public:
	Rigidbody* rigid = Rigidbody::Create(this);
	Vector<float> dir = {0,0};
	float speed = 9;


	Bullet(Scene* scene, Sprite* sprite);
	~Bullet();

	void Update() override;

	void OnHitTile() override;

	void OnTrigger(Collision other) override;

};

