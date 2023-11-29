#include "Test.h"
#include "ResourceManager.h"

Test::Test(noa::Scene * scene):noa::Actor(scene)
{
	this->tag = "Test";
}

Test::~Test()
{

}

void Test::Start()
{
	
}

void Test::Update()
{
	if (isHand&&noa::Input::GetMouseKeyUp(noa::MouseButton::LEFT_BUTTON))
	{
		isHand = false;
	}

	if (isHand) 
	{
		camera->SetFollow(this);
		noa::Vector<double> mousePos = noa::Input::GetMousePosition();
		noa::Vector<float> worldPos = camera->ScreenPointToWorld(mousePos.x,mousePos.y);
		
		noa::Vector<float> velocity = worldPos - this->transform.position;

		this->rigid->velocity = velocity*10.0f;

	}

}

void Test::OnMouseStay()
{
	if (noa::Input::GetMouseKeyDown(noa::MouseButton::LEFT_BUTTON))
	{
		isHand = true;
	}
}
