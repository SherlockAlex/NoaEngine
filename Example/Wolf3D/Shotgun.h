#pragma once
#include "Gun.h"
class Shotgun :public Gun
{
protected:
	Player* player = nullptr;
	Shotgun(Player* player, int* bulletCount,FreeCamera* camera);
	Image* image = nullptr;
public:
	static Shotgun* Create(Player* player, int* bulletCount, FreeCamera* camera);

	void OnEnter() override;
	void Update() override;
	void OnExit() override;

	void Delete() override;

};

