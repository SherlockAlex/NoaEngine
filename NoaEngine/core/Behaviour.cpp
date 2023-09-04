#include "Behaviour.h"
#include "NoaEngine.h"

#include <queue>
#include <list>

namespace noa {
	vector <Behaviour*> behaviours;
	//queue<Behaviour*> destroyBehaviours;

	void DestroyBehaviour(Behaviour* behaviour) {
		/*auto it = std::find(behaviours.begin(), behaviours.end(), behaviour);
		if (it != behaviours.end())
		{
			behaviours.erase(it);
		}*/
		//delete behaviour;
		
		for (int i = 0;i<behaviours.size();i++)
		{
			if (behaviours[i]==behaviour)
			{
				behaviours[i] = nullptr;
				Debug("Behaviour is destroy");
			}
		}

		

	}

	Behaviour::Behaviour()
	{
		behaviours.push_back(this);
		//behaviours.push_back(this);
	}

	Behaviour::~Behaviour()
	{
		DestroyBehaviour(this);
	}

	void Behaviour::Destroy()
	{
		DestroyBehaviour(this);
	}

	//void Destroy(Behaviour* behaviour)
	//{
	//	
	//	//destroyBehaviours.push(behaviour);
	//	//delete behaviour;
	//}

}

