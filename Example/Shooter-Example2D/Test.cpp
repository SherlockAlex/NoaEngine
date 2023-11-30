#include "Test.h"
#include "ResourceManager.h"

static Test* hold = nullptr;
static bool isMouseLeft = false;

Test::Test(noa::Scene * scene):noa::Actor(scene)
{
	this->tag = "Test";
	if (hold == nullptr) 
	{
		hold = this;
	}
}

Test::~Test()
{

}

void Test::Start()
{
	
}

void Test::Update()
{
	//ִ�и��������߼���ͬʱһ��ֻ�ܸ�һ��
	if (isMouseLeft
		&&noa::Input::GetMouseKeyUp(noa::MouseButton::LEFT_BUTTON)) 
	{
		isMouseLeft = false;
	}
	
	if (isMouseLeft) 
	{
		noa::Vector<double> mousePos = noa::Input::GetMousePosition();
		noa::Vector<float> worldPos = camera->ScreenPointToWorld(mousePos.x,mousePos.y);
		noa::Vector<float> velocity = worldPos - hold->transform.position;
		hold->rigid->velocity = velocity * 15.0f;
	}
}

void Test::OnMouseStay()
{

	//Ҫ������갴�µ�˲��

	if (!isMouseLeft
		&&noa::Input::GetMouseKeyDown(noa::MouseButton::LEFT_BUTTON))
	{
		isMouseLeft = true;
		hold = this;
		camera->SetFollow(this);
	}
}

