#include "BulletPool.h"
#include "BulletFactory.h"

using namespace noa;

BulletPool::BulletPool()
{
	this->SetFactory(bulletFactory.get());

	this->Prewarm(10);

}

std::shared_ptr<BulletPool> bulletPool;


