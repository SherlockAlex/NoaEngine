#include "UIMove.h"

UIMove::UIMove(noa::UIBody* body):noa::UIScript(body)
{

}

UIMove::~UIMove() {

}

void UIMove::Update() {
	//ÒÆ¶¯
	noa::UIBody* body = GetBody();
	if (body->GetHandle())
	{
		noa::Vector<double> mousePosition =
			noa::Input::GetMousePosition();
		if (noa::Input::GetMouseKeyHold(noa::MouseButton::LEFT_BUTTON)) 
		{
			body->SetGlobalPosition(
				mousePosition.x, 
				mousePosition.y
			);
		}
		//noa::Debug::Log(noa::ToString<double>(mousePosition));

	}

	

	/*body->SetGlobalPositionByAnchor(
		noa::Screen::width/2, 
		noa::Screen::height/2
	);*/

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