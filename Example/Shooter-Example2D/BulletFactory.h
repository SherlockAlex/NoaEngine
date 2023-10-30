#pragma once
#include "Engine.h"
#include "Bullet.h"

class BulletFactory:public noa::ActorFactory<Bullet>
{
public:
	Bullet* Create() override;
};

extern std::shared_ptr<BulletFactory> bulletFactory;