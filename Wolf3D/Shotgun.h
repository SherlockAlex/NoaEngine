#pragma once
#include "Gun.h"
class Shotgun :public Gun
{
protected:
	Player* player = nullptr;
	Shotgun(Player* player, int* bulletCount,FreeCamera* camera);
public:
	static Shotgun* Create(Player* player, int* bulletCount, FreeCamera* camera);

	void Update() override;
	void Delete() override;

};

