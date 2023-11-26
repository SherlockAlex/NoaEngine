#include "UIMove.h"

UIMove::UIMove(noa::UIBody* body):noa::UIScript(body)
{

}

UIMove::~UIMove() {

}

void UIMove::Update() {
	//ÒÆ¶¯
	noa::UIBody* body = GetBody();
	if (body->GetHandle()
		&& noa::Input::GetMouseKeyDown(noa::MouseButton::LEFT_BUTTON))
	{
		isHold = true;
	}

	noa::Vector<double> mousePosition =
		noa::Input::GetMousePosition();
	if (isHold)
	{
		body->SetGlobalPosition(
			mousePosition.x,
			mousePosition.y
		);
	}

	if (isHold&&noa::Input::GetMouseKeyUp(noa::MouseButton::LEFT_BUTTON))
	{
		isHold = false;
	}

}

void UIMove::OnEnter() {
	noa::Debug::Log("on enter");
}

void UIMove::OnStay() {

}

void UIMove::OnExit()
{
	noa::Debug::Log("on exit");
}