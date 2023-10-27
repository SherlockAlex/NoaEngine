#include "BulletFactory.h"
#include "Bullet.h"
#include "GameEngine.h"

using namespace noa;

Bullet* BulletFactory::Create()
{
	return NObject<Bullet>::Create<Scene*>(sceneManager.GetActiveScene());
}

std::shared_ptr<BulletFactory> bulletFactory;