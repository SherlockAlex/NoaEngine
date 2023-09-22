#pragma once
#include "Gun.h"
class Pistol :public Gun
{
protected:
	Pistol(int* bulletCount, FreeCamera* camera);

public:
	static Pistol* Create(int* bulletCount, FreeCamera* camera);
	void Update() override;
	void Delete() override;
};

