#include "GameObject.h"
#include "NoaEngine.h"



namespace noa {
	vector<GameObjectBuffer> gameObjects;

	void DestroyGameObject(GameObject* gameObject) {
		/*for (int i = 0;i<gameObjects.size();i++)
		{
			if (gameObjects[i].object == gameObject)
			{
				gameObjects[i].object = nullptr;
				Debug("GameObject has been destroy");
			}
		}*/

		for (auto it = gameObjects.begin(); it != gameObjects.end(); ) {
			if (it->object == gameObject) {
				it = gameObjects.erase(it);
				Debug("GameObject has been destroy");
			}
			else {
				++it;
			}
		}

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
		DestroyGameObject(this);
		Behaviour::~Behaviour();
	}

	void GameObject::Destroy()
	{
		SetActive(false);
		DestroyGameObject(this);
	}

	//void Destroy(GameObject* gameObject)
	//{
	//	//delete gameObject;
	//}

}

