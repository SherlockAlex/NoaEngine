#pragma once
#include "Gun.h"
class Pistol :public Gun
{
protected:
	Pistol(Player* player, int* bulletCount, FreeCamera* camera);

public:
	static Pistol* Create(Player* player, int* bulletCount, FreeCamera* camera);
	void Update() override;
	void Delete() override;
};

