#pragma once
#include "Gun.h"
class M4A1:public Gun
{
protected:
	M4A1(Player* player, int* bulletCount, FreeCamera* camera);
public:
	static M4A1* Create(Player* player, int* bulletCount, FreeCamera* camera);

	void Update() override;
	void Delete() override;
};

