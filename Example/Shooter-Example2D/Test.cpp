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
	//执行跟随鼠标的逻辑，同时一次只能跟一个
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

	//要的是鼠标按下的瞬间

	if (!isMouseLeft
		&&noa::Input::GetMouseKeyDown(noa::MouseButton::LEFT_BUTTON))
	{
		isMouseLeft = true;
		hold = this;
		camera->SetFollow(this);
	}
}

