#include "GameObject.h"
#include "NoaEngine.h"
#include <thread>


namespace noa {
	vector<GameObjectBuffer> gameObjects;

	void DestroyGameObject(GameObject* gameObject) {

		for (auto& object:gameObjects)
		{
			if (object.object == gameObject)
			{
				object.distanceToPlayer = -1;
				object.object = nullptr;
			}
		}

		/*for (auto it = gameObjects.begin(); it != gameObjects.end(); ) {
			if (it->object == gameObject) {
				it->distanceToPlayer = -1;
				it = gameObjects.erase(it);
				Debug("GameObject has been destroy");
			}
			else {
				++it;
			}
		}*/

	}

	GameObject::GameObject(Sprite * sprite) :Behaviour()
	{
		this->sprite = sprite;
		gameObjects.push_back({this,0});
	}

	GameObject::GameObject(Sprite * sprite, Vector<float> startPosition) :Behaviour()
	{
		this->sprite = sprite;
		this->transform.position = startPosition;
		gameObjects.push_back({ this,0 });
	}

	GameObject::~GameObject()
	{
		//DestroyGameObject(this);
		//DestroyGameObject(this);

		thread destroyGameObject([this]() {
			DestroyGameObject(this);
			});
		destroyGameObject.detach();
		Behaviour::~Behaviour();
	}

	void GameObject::Destroy()
	{
		SetActive(false);

		thread destroyGameObject([this]() {
			DestroyGameObject(this);
			});
		destroyGameObject.detach();
		//DestroyGameObject(this);
	}

	//void Destroy(GameObject* gameObject)
	//{
	//	//delete gameObject;
	//}

}

