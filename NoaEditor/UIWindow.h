#pragma once
#include <functional>

#include "NoaEditor.h"

class UIWindow final:noa::UIComponent
{
private:
	UIBODY(UIWindow)
private:
	UIWindow(noa::UIContainer* container);
	~UIWindow();

public:
	void AddUIComponent(
		std::function<void(noa::UIContainer*)> func);

private:
	noa::UIContainer* container = nullptr;
	noa::Image* background = nullptr;
};

