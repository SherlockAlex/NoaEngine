#pragma once
#include "Gun.h"
class Shotgun :public Gun
{
public:
	Shotgun(int* bulletCount,FreeCamera* camera);

	void Update() override;

};

