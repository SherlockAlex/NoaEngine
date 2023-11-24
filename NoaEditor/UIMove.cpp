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
			body->SetGlobalPosition(mousePosition.x, mousePosition.y);
		}
	}
}

UIMove* UIMove::Create(noa::UIBody* body)
{
	if (body==nullptr) 
	{
		return nullptr;
	}
	return new UIMove(body);
}