#include "Actor.h"
#include "NoaEngine.h"

#include <queue>
#include <list>

namespace noa 
{

	void DestroyBehaviour(Actor* behaviour)
	{
		//sceneManager.RemoveActor(behaviour);
		Scene* activeScene = behaviour->GetActiveScene();
		activeScene->RemoveActor(behaviour);

		Debug("Behaviour "+to_string(behaviour->GetHash()) + " is destroy");
	}

	// ��ʼ����̬������
	size_t Actor::nextId = 0;

	Actor::Actor(Scene* activeScene)
	{
		id = GetNextId();
		this->activeScene = activeScene;
		if (activeScene!=nullptr) 
		{
			activeScene->AddActor(this);
		}
		
		//ִ�н���actor��ӵ�activeScene���б���

		//sceneManager.AddActor(this);
	}

	Actor::~Actor()
	{
		activeScene->RemoveActor(this);
		//DestroyBehaviour(this);
	}

	//������Ϸ��Ʒ
	void Actor::Destroy()
	{
		OnDisable();
		activeScene->RemoveActor(this);
		//DestroyBehaviour(this);
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

	Scene* Actor::GetActiveScene()
	{
		return activeScene;
	}

	void Actor::SetActiveScene(Scene* scene)
	{
		this->activeScene = scene;
		scene->AddActor(this);
	}

	void Actor::OnTrigger(Collision collision)
	{
	}

}

