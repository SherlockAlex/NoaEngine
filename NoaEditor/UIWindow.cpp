#include "UIWindow.h"

UIWindow::UIWindow(noa::UIContainer* container)
	:noa::UIComponent(container)
{
	this->container = noa::UIContainer::Create(container)
		->SetAnchor(0.5f,0.5f)
		.SetVisiable(true)
		.Apply();
	this->background = noa::Image::Create(this->container)
		->SetSize(400,400)
		.SetAnchor(0.5f,0.5f)
		.Apply();
}

UIWindow::~UIWindow() {

}

void UIWindow::AddUIComponent(
	std::function<void(noa::UIContainer*)> func)
{
	func(this->container);
}

