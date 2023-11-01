#include "Test.h"
#include "ResourceManager.h"

Test::Test(noa::Scene * scene):noa::Actor(scene)
{
	this->tag = "Test";

	this->spriteRenderer->SetSprite(&ResourceManager::playerImage);
	this->spriteRenderer->SetScale(2, 2);
	this->rigid->useCollision = true;
	this->rigid->useGravity = true;
	this->transform.position = { 5,3 };
	this->tileCollider->SetScale(1.5f, 1.7f);
	this->collider->radius = 0.7f;
	this->rigid->gravityWeight = 5.5f;
	
	this->rigid->bodyType = noa::BodyType::DYNAMIC;

}

Test::~Test()
{

}

void Test::Start()
{
	
}

void Test::Update()
{
	
	noa::Debug::Log(std::to_string(static_cast<int>(this->rigid->bodyType)));
	//rigid->velocity = {};
}
