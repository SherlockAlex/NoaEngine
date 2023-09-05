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
		
		/*for (int i = 0;i<behaviours.size();i++)
		{
			if (behaviours[i]==behaviour)
			{
				behaviours[i] = nullptr;
				Debug("Behaviour is destroy");
			}
		}*/

		behaviours[behaviour->hashCode] = nullptr;
		Debug("Behaviour "+to_string(behaviour->hashCode) + " is destroy");

		

	}

	Behaviour::Behaviour()
	{
		behaviours.push_back(this);
		for (int i = 0;i<behaviours.size();i++)
		{
			if (behaviours[i]==this)
			{
				this->hashCode = i;
			}
		}
		//behaviours.push_back(this);
	}

	Behaviour::~Behaviour()
	{
		DestroyBehaviour(this);
	}

	void Behaviour::Destroy()
	{
		OnDisable();
		DestroyBehaviour(this);
	}

	void Behaviour::SetActive(bool value)
	{
		isActive = value;
		if (isActive)
		{
			OnEnable();
			return;
		}
		OnDisable();

	}

	bool Behaviour::GetActive()
	{
		return isActive;
	}

	//void Destroy(Behaviour* behaviour)
	//{
	//	
	//	//destroyBehaviours.push(behaviour);
	//	//delete behaviour;
	//}

}

