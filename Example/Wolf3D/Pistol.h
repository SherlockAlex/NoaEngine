#pragma once
#include "Gun.h"
class Pistol :public Gun
{
protected:
	Player* player = nullptr;
	Pistol(Player* player, int* bulletCount, FreeCamera* camera);
	Image* image = nullptr;
public:
	static Pistol* Create(Player* player, int* bulletCount, FreeCamera* camera);

	void OnEnter() override;
	void Update() override;
	void OnExit() override;

	void Delete() override;
};

