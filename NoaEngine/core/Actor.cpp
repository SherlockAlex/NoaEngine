#include "Actor.h"
#include "NoaEngine.h"
#include "ActorComponent.h"

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
		DestroyComponent();
		Debug("Remove actor");
	}

	void Actor::AddComponent(ActorComponent* component)
	{
		if (component == nullptr)
		{
			return;
		}
		components.push_back(component);

		auto last = std::unique(components.begin(), components.end());
		components.erase(last,components.end());

	}

	void Actor::ComponentAwake()
	{
		for (int i = 0;i<components.size();i++)
		{
			if (components[i]== nullptr) 
			{
				continue;
			}
			components[i]->Awake();
		}
	}

	void Actor::ComponentOnEnable()
	{
		for (int i = 0; i < components.size(); i++)
		{
			if (components[i] == nullptr)
			{
				continue;
			}
			components[i]->OnEnable();
		}
	}

	void Actor::ComponentStart()
	{
		for (int i = 0; i < components.size(); i++)
		{
			if (components[i] == nullptr)
			{
				continue;
			}
			components[i]->Start();
		}
	}

	void Actor::ComponentUpdate()
	{
		for (int i = 0; i < components.size(); i++)
		{
			if (components[i] == nullptr)
			{
				continue;
			}
			components[i]->Update();
		}
	}

	void Actor::ComponentOnDisable()
	{
		for (int i = 0; i < components.size(); i++)
		{
			if (components[i] == nullptr)
			{
				continue;
			}
			components[i]->OnDisable();
		}
	}

	void Actor::ComponentOnDestroy()
	{
		for (int i = 0; i < components.size(); i++)
		{
			if (components[i] == nullptr)
			{
				continue;
			}
			components[i]->OnDestroy();
		}
	}

	void Actor::DestroyComponent()
	{
		if (components.empty()) 
		{
			return;
		}

		for (int i = 0; i < components.size(); i++)
		{
			if (components[i] == nullptr)
			{
				continue;
			}
			components[i]->OnDestroy();
			components[i]->Delete();
		}
		components.clear();
	}

	Actor* Actor::Create(Scene* activeScene)
	{
		return new Actor(activeScene);
	}

	void Actor::Delete()
	{
		delete this;
	}

	//销毁游戏物品
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

