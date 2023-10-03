#include "Actor.h"
#include "NoaEngine.h"
#include "ActorComponent.h"
#include "Physics.h"

#include <queue>
#include <list>

using namespace std;

namespace noa 
{
	void DestroyBehaviour(Actor* behaviour);
}

void noa::DestroyBehaviour(noa::Actor* behaviour)
{
	//sceneManager.RemoveActor(behaviour);
	noa::Scene* activeScene = behaviour->GetActiveScene();
	activeScene->RemoveActor(behaviour);

	noa::Debug("Behaviour " + to_string(behaviour->GetHash()) + " is destroy");
}

// 初始化静态计数器
size_t noa::Actor::nextId = 0;

noa::Actor::Actor(noa::Scene* activeScene)
{
	id = GetNextId();
	this->activeScene = activeScene;
	if (activeScene != nullptr)
	{
		activeScene->AddActor(this);
	}
}

noa::Actor::~Actor()
{
	//删除此actor
	if (rigidbody!=nullptr)
	{
		rigidbody->actor = nullptr;
		delete rigidbody;
	}

	for (auto & component:components)
	{
		component->DeleteActorEvent();
	}

	DestroyComponent();
	DestroyRigidbody();
	Debug("Remove actor");
}

void noa::Actor::AddComponent(noa::ActorComponent* component)
{
	if (component == nullptr)
	{
		return;
	}
	components.push_back(component);

	//添加完组件后开始排查重复项


}

void noa::Actor::ComponentAwake()
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i] == nullptr)
		{
			continue;
		}
		components[i]->Awake();
	}
}

void noa::Actor::ComponentOnEnable()
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

void noa::Actor::ComponentStart()
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

void noa::Actor::ComponentUpdate()
{
	if (rigidbody!=nullptr&&!rigidbody->isRemoved)
	{
		rigidbody->Update();
		this->activeScene->rigidbodys.push_back(rigidbody);
	}

	for (int i = 0; i < components.size(); i++)
	{
		if (components[i] == nullptr || !components[i]->GetActive())
		{
			continue;
		}
		components[i]->Update();
	}
}

void noa::Actor::ComponentOnDisable()
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

void noa::Actor::ComponentOnDestroy()
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

void noa::Actor::DestroyComponent()
{
	if (components.empty())
	{
		return;
	}

	std::sort(components.begin(), components.end());
	auto last = std::unique(components.begin(), components.end());
	components.erase(last, components.end());

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

void noa::Actor::AddRigidbody(Rigidbody* rigid)
{
	// 为角色添加刚体
	if (rigid == nullptr)
	{
		return;
	}
	this->rigidbody = rigid;

}

void noa::Actor::RemoveRigidbody()
{
	rigidbody = nullptr;
}

void noa::Actor::DestroyRigidbody()
{
	// 移除刚体
	if (rigidbody == nullptr)
	{
		return;
	}

	rigidbody->Destroy();
	rigidbody = nullptr;

}

noa::Actor* noa::Actor::Create(Scene* activeScene)
{
	return new Actor(activeScene);
}

void noa::Actor::Delete()
{
	delete this;
}

//销毁游戏物品
void noa::Actor::Destroy()
{
	SetActive(false);
	DestroyRigidbody();
	activeScene->RemoveActor(this);
}

void noa::Actor::SetActive(bool value)
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

bool noa::Actor::GetActive()
{
	return isActive;
}

noa::Scene* noa::Actor::GetActiveScene()
{
	return activeScene;
}

void noa::Actor::SetActiveScene(noa::Scene* scene)
{
	this->activeScene = scene;
	scene->AddActor(this);
}

void noa::Actor::OnTrigger(noa::Collision collision)
{
}


