#include "Actor.h"
#include "NoaEngine.h"

#include <queue>
#include <list>

using namespace std;

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
		Debug("Remove actor");
	}

	Actor* Actor::Create(Scene* activeScene)
	{
		return new Actor(activeScene);
	}

	void Actor::Delete()
	{
		delete this;
	}

	//������Ϸ��Ʒ
	void Actor::Destroy()
	{
		OnDisable();
		activeScene->RemoveActor(this);
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

