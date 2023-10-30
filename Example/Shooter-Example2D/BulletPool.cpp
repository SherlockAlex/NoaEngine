#include "BulletPool.h"

BulletPool::BulletPool() :ActorPool<Bullet>() {

}

BulletPool::~BulletPool() {

}

std::shared_ptr<BulletPool> bulletPool = std::make_shared<BulletPool>();
