#pragma once
#include "GameEngine.h"

using namespace noa;

class Gun
{
public:
	int damage = 10;
	int takeBullet = 1;
	FreeCamera* camera = nullptr;
	Sprite * sprite = nullptr;
	Animation * animation = nullptr;
	Audio* audio = nullptr;

	int* bulletCount = nullptr;

public:
	Gun(int * bulletCount,FreeCamera * camera);
protected:
	virtual ~Gun();
public:
	virtual void Delete();

	virtual void Update();
	virtual void Shoot();
};

