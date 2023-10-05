#pragma once
#include "GameEngine.h"

class Bullet;
class BulletPool:public noa::ActorPool<Bullet>
{
public:
	BulletPool();
};

extern std::shared_ptr<BulletPool> bulletPool;
