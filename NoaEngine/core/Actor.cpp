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

	// 初始化静态计数器
	size_t Actor::nextId = 0;

	Actor::Actor(Scene* activeScene)
	{
		id = GetNextId();
		this->activeScene = activeScene;
		if (activeScene!=nullptr) 
		{
			activeScene->AddActor(this);
		}
		
		//执行将本actor添加到activeScene的列表中

		//sceneManager.AddActor(this);
	}

	Actor::~Actor()
	{
		activeScene->RemoveActor(this);
		//DestroyBehaviour(this);
	}

	//销毁游戏物品
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

