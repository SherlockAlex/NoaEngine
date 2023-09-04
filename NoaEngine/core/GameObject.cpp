#include "GameObject.h"
#include "NoaEngine.h"



namespace noa {
	vector<GameObject*> gameObjects;

	void DestroyGameObject(GameObject* gameObject) {
		/*auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
		if (it != gameObjects.end())
		{
			gameObjects.erase(it);
		}*/
		for (int i = 0;i<gameObjects.size();i++)
		{
			if (gameObjects[i] == gameObject)
			{
				gameObjects[i] = nullptr;
				Debug("GameObject has been destroy");
			}
		}

		

	}

	GameObject::GameObject(Sprite * sprite) :Behaviour()
	{
		this->sprite = sprite;
		gameObjects.push_back(this);
	}

	GameObject::GameObject(Sprite * sprite, Vector<float> startPosition) :Behaviour()
	{
		this->sprite = sprite;
		this->transform.position = startPosition;
		gameObjects.push_back(this);
	}

	GameObject::~GameObject()
	{
		//DestroyGameObject(this);
		DestroyGameObject(this);
		Behaviour::~Behaviour();
	}

	void GameObject::Destroy()
	{
		DestroyGameObject(this);
	}

	//void Destroy(GameObject* gameObject)
	//{
	//	//delete gameObject;
	//}

}

