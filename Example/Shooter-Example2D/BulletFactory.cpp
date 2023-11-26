#include "BulletFactory.h"

std::shared_ptr<noa::ActorFactory<Bullet>> bulletFactory 
	= std::make_shared<noa::ActorFactory<Bullet>>();