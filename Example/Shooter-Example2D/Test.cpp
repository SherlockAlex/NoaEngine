#include "Test.h"
#include "ResourceManager.h"

Test::Test(noa::Scene * scene):noa::Actor(scene)
{
	this->tag = "Test";

	this->spriteRenderer->SetSprite(&ResourceManager::playerImage);
	this->spriteRenderer->SetScale(2, 2);
	this->rigid->useCollision = true;
	this->rigid->useGravity = false;
	this->rigid->damping = 0;
	this->transform.position = { 3,3 };
	this->tileCollider->SetScale(1.5f, 1.7f);
	this->collider->radius = 0.7f;
	this->rigid->gravityWeight = 5.5f;
	
}

Test::~Test()
{

}

void Test::Start()
{

}

void Test::Update()
{
	//rigid->velocity = {};
}
