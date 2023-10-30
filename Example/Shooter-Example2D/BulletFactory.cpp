#include "BulletFactory.h"

Bullet* BulletFactory::Create() {
	//´´½¨×Óµ¯
	return noa::NObject<Bullet>::Create(noa::sceneManager.GetActiveScene());
}

std::shared_ptr<BulletFactory> bulletFactory = std::make_shared<BulletFactory>();