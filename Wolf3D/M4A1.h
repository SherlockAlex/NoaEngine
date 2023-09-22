#pragma once
#include "Gun.h"
class M4A1:public Gun
{
protected:
	M4A1(int* bulletCount, FreeCamera* camera);
public:
	static M4A1* Create(int* bulletCount, FreeCamera* camera);

	void Update() override;
	void Delete() override;
};

