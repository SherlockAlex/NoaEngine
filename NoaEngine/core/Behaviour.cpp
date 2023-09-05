#include "Behaviour.h"
#include "NoaEngine.h"

#include <queue>
#include <list>

namespace noa {
	unordered_map<size_t,Behaviour*> behaviours;
	//queue<Behaviour*> destroyBehaviours;

	void DestroyBehaviour(Behaviour* behaviour) 
	{
		behaviours[behaviour->GetHash()] = nullptr;
		Debug("Behaviour "+to_string(behaviour->GetHash()) + " is destroy");
	}

	// 初始化静态计数器
	size_t Behaviour::nextId = 0;

	Behaviour::Behaviour()
	{
		id = GetNextId();
		behaviours[GetHash()] = this;
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

}

