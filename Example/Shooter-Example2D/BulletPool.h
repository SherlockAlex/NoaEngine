#pragma once

#include "Bullet.h"
#include "Engine.h"
class BulletPool:public noa::ActorPool<Bullet>
{
	//����BulletFactor
public:
	BulletPool();
	~BulletPool();
};
extern std::shared_ptr<BulletPool> bulletPool;

