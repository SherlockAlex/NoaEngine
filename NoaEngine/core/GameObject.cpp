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

	GameObject::~GameObject()
	{

		thread destroyGameObject([this]() {
			//DestroyGameObject(this);
			this->GetActiveScene()->RemoveGameObject(this);
			});
		destroyGameObject.detach();
		Actor::~Actor();
	}

	void GameObject::Destroy()
	{
		SetActive(false);
		

		thread destroyGameObject([this]() {
			this->GetActiveScene()->RemoveGameObject(this);
		});
		destroyGameObject.detach();
		Actor::Destroy();

	}

}

