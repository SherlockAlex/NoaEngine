#include "BulletFactory.h"

Bullet* BulletFactory::Create() {
	//´´½¨×Óµ¯
	if (!activeScene) 
	{
		return noa::NObject<Bullet>::Create(noa::sceneManager.GetActiveScene());
	}
	return noa::NObject<Bullet>::Create(activeScene);
}

std::shared_ptr<BulletFactory> bulletFactory = std::make_shared<BulletFactory>();