#pragma once
#include "Gun.h"
class Shotgun :public Gun
{
protected:
	Shotgun(int* bulletCount,FreeCamera* camera);
public:
	static Shotgun* Create(int* bulletCount, FreeCamera* camera);

	void Update() override;
	void Delete() override;

};

