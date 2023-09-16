#pragma once
#include "GameEngine.h"
using namespace noa;
class Bullet:public GameObject,public Rigidbody
{
public:
	Vector<float> dir = {0,0};
	float speed = 9;


	Bullet(Sprite* sprite);
	~Bullet();

	void Update() override;

	void OnHitTile() override;

	void OnTrigger(Collision other) override;

};

