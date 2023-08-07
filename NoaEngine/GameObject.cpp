#include "GameObject.h"
#include "NoaEngine.h"

vector<GameObject*> gameObjects;

void DestroyGameObject(GameObject* gameObject) {
	auto it = std::find(gameObjects.begin(), gameObjects.end(), gameObject);
	if (it != gameObjects.end())
	{
		gameObjects.erase(it);
	}

	Debug("GameObject±»Ïú»Ù");

}

GameObject::GameObject(Sprite sprite) :Behaviour()
{
	this->sprite = sprite;
	gameObjects.push_back(this);
}

GameObject::GameObject(Sprite sprite, Vector startPosition) :Behaviour() 
{
	this->sprite = sprite;
	this->position = startPosition;
	gameObjects.push_back(this);
}

GameObject::~GameObject()
{
	DestroyGameObject(this);
	Behaviour::~Behaviour();
}

void GameObject::Destroy()
{
	delete this;
}

