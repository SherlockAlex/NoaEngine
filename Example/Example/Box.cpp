#include "Box.h"

static Box* box = nullptr;

Box::Box(noa::Scene* scene):Actor(scene)
{
	noa::Sprite sprite("box.spr", 16, 16);
	this->spriteRenderer->SetSprite(&sprite);
	transform.position = { 0.5f,0.5f };
	if (box == nullptr) 
	{
		box = this;
	}
}

Box::~Box() 
{

}

void Box::Update() 
{

	if (isHandled&&noa::Input::GetMouseKeyDown(noa::MouseButton::LEFT_BUTTON)) 
	{
		noa::Vector<double> mousePos 
			= noa::Input::GetMousePosition();

		noa::Vector<float> worldPos
			= camera->ScreenPointToWorld(mousePos.x, mousePos.y);

		noa::Vector<float> direction
			= worldPos - transform.position;

		box->rigid->velocity = direction * 10;

	}

	if (noa::Input::GetMouseKeyUp(noa::MouseButton::LEFT_BUTTON)) 
	{
		box->isHandled = false;
	}

}

void Box::OnMouseStay() {
	//ץסbox
	if (noa::Input::GetMouseKeyDownOnce(
		noa::MouseButton::LEFT_BUTTON)) 
	{
		camera->SetFollow(this);
		box = this;
		box->isHandled = true;
	}


}