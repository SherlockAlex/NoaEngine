#include "Shooter.h"

noa::NoaEngine* noa::Application::CreateEngine() {
	Shooter* game = new Shooter();
	return game;
}
