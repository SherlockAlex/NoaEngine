#include "Actor.h"
#include "NoaEngine.h"
#include "ActorComponent.h"
#include "Physics.h"

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
		DestroyRigidbody();
		Debug("Remove actor");
	}

	void Actor::AddComponent(ActorComponent* component)
	{
		if (component == nullptr)
		{
			return;
		}
		components.push_back(component);

		std::sort(components.begin(), components.end());
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
			if (components[i] == nullptr||!components[i]->GetActive())
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

		std::sort(components.begin(),components.end());
		auto last = std::unique(components.begin(),components.end());
		components.erase(last,components.end());

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

	void Actor::AddRigidbody(Rigidbody* rigid)
	{
		if (rigid==nullptr)
		{
			return;
		}
		rigidbodys.push_back(rigid);

		std::sort(rigidbodys.begin(), rigidbodys.end());
		auto last = std::unique(rigidbodys.begin(), rigidbodys.end());
		rigidbodys.erase(last, rigidbodys.end());

	}

	void Actor::RemoveRigidbody(Rigidbody* rigid)
	{
		if (rigid == nullptr)
		{
			return;
		}

		std::sort(rigidbodys.begin(), rigidbodys.end());
		auto last = std::unique(rigidbodys.begin(), rigidbodys.end());
		rigidbodys.erase(last, rigidbodys.end());

		auto item = std::remove(rigidbodys.begin(),rigidbodys.end(),rigid);
		rigidbodys.erase(item,rigidbodys.end());

	}

	void Actor::DestroyRigidbody()
	{
		if (rigidbodys.empty()) 
		{
			return;
		}

		for (int i=0;i<rigidbodys.size();i++) 
		{
			if (rigidbodys[i]==nullptr)
			{
				continue;
			}
			rigidbodys[i]->Destroy();
			rigidbodys[i] = nullptr;
		}
		rigidbodys.clear();

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
		SetActive(false);
		//OnDisable();
		DestroyRigidbody();
		activeScene->RemoveActor(this);
	}

	void Actor::SetActive(bool value)
	{
		if (isActive == value) 
		{
			return;
		}
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

