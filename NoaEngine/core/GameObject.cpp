#include "GameObject.h"
#include "NoaEngine.h"
#include <thread>


namespace noa {
	//vector<GameObjectBuffer> gameObjects;

	void DestroyGameObject(GameObject* gameObject) {

		/*for (auto& object:gameObjects)
		{
			if (object.object == gameObject)
			{
				object.distanceToPlayer = -1;
				object.object = nullptr;
			}
		}*/

	}

	GameObject::GameObject(Scene* activeScene, Sprite * sprite) :Actor(activeScene)
	{
		this->sprite = sprite;
		this->activeScene->AddGameObject({this,0});
		//this.activeScene.AddGameObject()
		//gameObjects.push_back({this,0});
	}

	GameObject::GameObject(Scene* activeScene, Sprite * sprite, Vector<float> startPosition) :Actor(activeScene)
	{
		this->sprite = sprite;
		this->transform.position = startPosition;
		this->activeScene->AddGameObject({ this,0 });
		//gameObjects.push_back({ this,0 });
	}

	GameObject* GameObject::Create(Scene* activeScene, Sprite* sprite)
	{
		return new GameObject(activeScene, sprite);
	}

	GameObject* GameObject::Create(Scene* activeScene, Sprite* sprite, Vector<float> startPosition)
	{
		return new GameObject(activeScene, sprite, startPosition);
	}

	void GameObject::Delete()
	{
		delete this;
	}

	GameObject::~GameObject()
	{
		Actor::~Actor();
		Debug("Remove gameObject");
	}

	void GameObject::Destroy()
	{
		SetActive(false);
		this->GetActiveScene()->RemoveGameObject(this);
		Actor::Destroy();

	}

}

