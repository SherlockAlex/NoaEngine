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
	noa::Debug::Log(noa::ToString<float>(this->transform.position));
	/*if (transform.position.x<0) 
	{
		transform.position.x = 0;
		rigid->velocity.x = -0.5f*rigid->velocity.x;
	}else if (transform.position.x > 10)
	{
		transform.position.x = 10;
		rigid->velocity.x = -0.5f * rigid->velocity.x;
	}

	if (transform.position.y < 0)
	{
		transform.position.y = 0;
		rigid->velocity.y = -0.5f * rigid->velocity.y;
	}
	else if (transform.position.y > 10)
	{
		transform.position.y = 10;
		rigid->velocity.y = -0.5f * rigid->velocity.y;
	}*/

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