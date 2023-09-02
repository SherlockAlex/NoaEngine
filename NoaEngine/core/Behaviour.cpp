#include "Behaviour.h"
#include "NoaEngine.h"

namespace noa {
	vector <Behaviour*> behaviours;

	void DestroyBehaviour(Behaviour* behaviour) {
		auto it = std::find(behaviours.begin(), behaviours.end(), behaviour);
		if (it != behaviours.end())
		{
			behaviours.erase(it);
		}
		//delete behaviour;
		Debug("Behaviour is destroy");

	}

	Behaviour::Behaviour()
	{
		behaviours.push_back(this);
	}

	Behaviour::~Behaviour()
	{
		DestroyBehaviour(this);
	}

	void Destroy(Behaviour* behaviour)
	{
		delete behaviour;
	}

}

