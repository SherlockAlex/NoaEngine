#include "Behaviour.h"
#include "NoaEngine.h"

vector <Behaviour*> behaviours;

void DestroyBehaviour(Behaviour * behaviour) {
	auto it = std::find(behaviours.begin(), behaviours.end(), behaviour);
	if (it != behaviours.end())
	{
		behaviours.erase(it);
	}
	//delete behaviour;
	game.Debug("Behaviour被销毁");

}

Behaviour::Behaviour()
{
	behaviours.push_back(this);
}

void Behaviour::Start()
{
}

Behaviour::~Behaviour()
{
	DestroyBehaviour(this);
}

void Behaviour::Destroy()
{
	//删除这个对象
	delete this;
}
