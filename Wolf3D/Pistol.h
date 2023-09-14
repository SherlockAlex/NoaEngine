#pragma once
#include "Gun.h"
class Pistol :public Gun
{
public:
	Pistol(int* bulletCount, FreeCamera* camera);

	void Update() override;
};

