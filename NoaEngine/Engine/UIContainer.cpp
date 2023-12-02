#include <vector>
#include <algorithm>

#include "UIContainer.h"
#include "UIComponent.h"
#include "UIDocument.h"

noa::UIContainer::UIContainer(UIDocument* document)
{
	if (document)
	{
		document->AddUIContainer(this);
	}
	this->SetVisiable(false);
	noa::UIBody::SetLocalSize(0,0);
	noa::UIBody::anchor = { 0.0f,0.0f };

}

noa::UIContainer::UIContainer(UIContainer* father)
{
	if (father)
	{
		father->AddUIContainer(this);
	}
	this->SetVisiable(false);
	noa::UIBody::SetLocalSize(0, 0);
	noa::UIBody::anchor = { 0.5f,0.5f };
}

noa::UIContainer::~UIContainer() {

	//É¾³ý¿Ø¼þ
	DestroyUIContainer();
}

noa::UIContainer* noa::UIContainer::Create(noa::UIDocument* document)
{
	if (document == nullptr)
	{
		return nullptr;
	}

	UIContainer* container = new UIContainer(document);
	return container;

}

noa::UIContainer* noa::UIContainer::Create(noa::UIContainer* father)
{
	if (father == nullptr)
	{
		return nullptr;
	}
	UIContainer* container = new UIContainer(father);
	return container;
}

void noa::UIContainer::Delete(UIContainer*& ptr)
{
	delete this;
	ptr = nullptr;
}

noa::UIContainer& noa::UIContainer::SetLocalSize(int x,int y) {
	noa::UIBody::SetLocalSize(x,y);
	return *this;
}

noa::UIContainer& noa::UIContainer::SetID(const std::string& id)
{
	this->id = id;
	return *this;
}

noa::UIContainer& noa::UIContainer::SetLocalPosition(int x, int y)
{
	noa::UIBody::SetLocalPosition(x,y);
	return *this;
}

noa::UIContainer& noa::UIContainer::SetGlobalPosition(int x, int y)
{
	noa::UIBody::SetGlobalPosition(x,y);
	return *this;
}

noa::UIContainer& noa::UIContainer::SetVisiable(bool value)
{
	this->visiable = value;
	return *this;
}

noa::UIContainer& noa::UIContainer::SetAnchor(float x,float y) 
{
	this->anchor.x = x;
	this->anchor.y = y;
	return *this;
}

noa::UIContainer* noa::UIContainer::Apply() {
	return this;
}

void noa::UIContainer::AddUIComponent(UIComponent* component)
{
	if (component == nullptr)
	{
		return;
	}
	component->SetActiveInContainer(true);
	uiComponent.push_back(component);
}

void noa::UIContainer::AddUIContainer(UIContainer* container)
{
	if (container == nullptr)
	{
		return;
	}

	container->SetVisiable(true);
	subContainers.push_back(container);

}

size_t noa::UIContainer::GetContainerIndex() {
	return this->index;
}

void noa::UIContainer::Start() 
{
	for (auto& container:subContainers) 
	{
		if (container) 
		{
			container->Start();
		}
	}
	for (auto& component : uiComponent)
	{
		if (component != nullptr)
		{
			component->Start();
		}
	}
}

void noa::UIContainer::Update()
{
	
	if (!visiable)
	{
		return;
	}

	noa::UIBody::OnUpdate();

	UITransform childFatherTransform;
	childFatherTransform.position.x =
		globalTransform.position.x;
	childFatherTransform.position.y =
		globalTransform.position.y;
	childFatherTransform.size = transform.size;

	for (auto& component : uiComponent)
	{
		if (component == nullptr || !component->active)
		{
			continue;
		}
		component->fatherAnchor = anchor;
		component->SetFatherTransform(childFatherTransform);
		component->Update();
	}

	for (auto& container : subContainers)
	{
		if (container == nullptr || !container->visiable)
		{
			continue;
		}
		
		container->fatherAnchor = anchor;
		container->SetFatherTransform(childFatherTransform);
		container->Update();
	}

}

void noa::UIContainer::Render() {

	if (!visiable)
	{
		return;
	}

	for (auto& component : uiComponent)
	{
		if (component == nullptr || !component->active)
		{
			continue;
		}
		component->fatherTransform = this->globalTransform;
		component->Render();
	}

	//ÏÔÊ¾±³¾°
	for (auto& container : subContainers)
	{
		if (container == nullptr || !container->visiable)
		{
			continue;
		}
		container->fatherTransform = this->globalTransform;
		container->Render();
	}

	

}

void noa::UIContainer::DestroyUIContainer() {
	if (!this->uiComponent.empty())
	{
		//¶Ôcomponent½øÐÐÅÅÐò
		std::sort(uiComponent.begin(), uiComponent.end());
		auto last = std::unique(uiComponent.begin(), uiComponent.end());
		uiComponent.erase(last, uiComponent.end());

		for (auto& component : uiComponent)
		{
			if (component)
			{
				component->Delete(component);
			}
		}
	}

	uiComponent.clear();

	if (!subContainers.empty())
	{
		std::sort(subContainers.begin(), subContainers.end());
		auto last = std::unique(subContainers.begin(), subContainers.end());
		subContainers.erase(last, subContainers.end());

		for (auto& child : subContainers)
		{
			if (child)
			{
				child->Delete(child);
			}
		}
	}
	subContainers.clear();

}
