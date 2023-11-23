#include "UIComponent.h"
#include "UIContainer.h"

noa::UIComponent::UIComponent(noa::UIContainer* container)
{
	if (container)
	{
		container->AddUIComponent(this);
	}
}

noa::UIComponent::~UIComponent()
{

}

void noa::UIComponent::Delete(UIComponent*& ptr)
{
	delete this;
	ptr = nullptr;
}

void noa::UIComponent::SetGlobalPosition(int x, int y) {
	//计算值，然后换算成position

	transform.position.x =
		static_cast<int>(x + anchor.x * transform.size.x
		- fatherTransform.position.x);

	transform.position.y =
		static_cast<int>(y + anchor.y * transform.size.y
		- fatherTransform.position.y);

}

void noa::UIComponent::SetActiveInContainer(bool active)
{
	this->active = active;
}

bool noa::UIComponent::GetActive()
{
	return active;
}
