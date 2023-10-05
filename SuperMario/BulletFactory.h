#pragma once
#include "GameEngine.h"

class Bullet;
class BulletFactory:public noa::ActorFactory<Bullet>
{
public:
	Bullet* Create() override;
};

extern std::shared_ptr<BulletFactory> bulletFactory;