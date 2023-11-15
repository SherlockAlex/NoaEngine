#include "Test.h"
#include "ResourceManager.h"

Test::Test(noa::Scene * scene):noa::Actor(scene)
{
	this->tag = "Test";

	this->rigid->useGravity = true;
	
	this->rigid->gravityWeight = 5.5f;
	this->rigid->damping = 0.0f;
	this->rigid->SetBodyType(noa::BodyType::DYNAMIC);

	this->rigid->bounce = 0.5f;
	this->rigid->friction = 0.8f;

}

Test::~Test()
{

}

void Test::Start()
{
	
}

void Test::Update()
{
	
}
