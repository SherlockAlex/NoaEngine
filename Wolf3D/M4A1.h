#pragma once
#include "Gun.h"
class M4A1:public Gun
{
public:
	M4A1(int* bulletCount, FreeCamera* camera);

	void Update() override;
};

