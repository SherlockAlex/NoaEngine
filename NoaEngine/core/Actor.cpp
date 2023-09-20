#include "Actor.h"
#include "NoaEngine.h"

#include <queue>
#include <list>

namespace noa 
{
	//unordered_map<size_t, Actor*> behaviours;

	void DestroyBehaviour(Actor* behaviour)
	{
		sceneManager.RemoveActor(behaviour);
		//behaviours[behaviour->GetHash()] = nullptr;
		Debug("Behaviour "+to_string(behaviour->GetHash()) + " is destroy");
	}

	// ��ʼ����̬������
	size_t Actor::nextId = 0;

	Actor::Actor()
	{
		id = GetNextId();
		sceneManager.AddActor(this);
		//behaviours[GetHash()] = this;
	}

	Actor::~Actor()
	{
		DestroyBehaviour(this);
	}

	void Actor::Awake()
	{

	}

	//������Ϸ��Ʒ
	void Actor::Destroy()
	{
		OnDisable();
		DestroyBehaviour(this);
	}

	void Actor::SetActive(bool value)
	{
		isActive = value;
		if (isActive)
		{
			OnEnable();
			return;
		}
		OnDisable();

	}

	bool Actor::GetActive()
	{
		return isActive;
	}

}

