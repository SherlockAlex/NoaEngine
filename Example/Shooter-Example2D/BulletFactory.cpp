#include "BulletFactory.h"

Bullet* BulletFactory::Create() {
	//�����ӵ�
	return noa::NObject<Bullet>::Create(noa::sceneManager.GetActiveScene());
}

std::shared_ptr<BulletFactory> bulletFactory = std::make_shared<BulletFactory>();