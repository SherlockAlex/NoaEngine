#include "UIDocument.h"
#include "UIContainer.h"
#include "UIHub.h"

noa::UIDocument::UIDocument(noa::Actor* actor)
	:noa::ActorComponent(actor)
{
	noa::UIHub::documents.push_back(this);
}

noa::UIDocument::~UIDocument()
{
	noa::UIHub::RemoveDocument(this);
	while (!containerStack.empty())
	{
		containerStack.pop();
	}
	for (auto& container : containerList)
	{
		container->Delete(container);
	}
}

void noa::UIDocument::AddUIContainer(UIContainer* container)
{
	if (container == nullptr)
	{
		return;
	}
	container->index = containerList.size();
	this->containerList.push_back(container);

}

noa::UIDocument* noa::UIDocument::Create(noa::Actor* actor)
{
	if (actor == nullptr) 
	{
		noa::Debug::Error("create UIDocument failed");
		return nullptr;
	}
	return noa::NObject<noa::UIDocument>::Create(actor);
}

noa::UIDocument& noa::UIDocument::SetID(const std::string& id)
{
	this->id = id;
	return *this;
}

noa::UIDocument* noa::UIDocument::Apply() {
	return this;
}

void noa::UIDocument::Display(size_t index)
{
	if (index >= containerList.size() || containerList[index] == nullptr)
	{
		return;
	}
	containerList[index]->SetVisiable(true);
	this->containerStack.push(containerList[index]);
}

void noa::UIDocument::Display(const std::string& id)
{
	UIContainer* container = this->GetElementByID<UIContainer>(id);
	if (!container)
	{
		return;
	}
	Display(container->GetContainerIndex());
}

void noa::UIDocument::Display(noa::UIContainer* container)
{
	if (!container)
	{
		return;
	}

	Display(container->GetContainerIndex());
}

void noa::UIDocument::Close() {
	if (containerStack.empty())
	{
		return;
	}
	containerStack.top()->SetVisiable(false);
	containerStack.pop();
}

void noa::UIDocument::Start() {
	for (auto& container:containerList) 
	{
		if (container) 
		{
			container->Start();
		}
	}
}

void noa::UIDocument::Update()
{
	if (containerStack.empty())
	{
		return;
	}

	//控制逻辑只控制栈顶
	if (!containerStack.empty())
	{
		containerStack.top()->Update();
	}

}

void noa::UIDocument::Render() {
	for (auto& container : containerList)
	{
		//越往前的越先绘制
		if (container && container->visiable)
		{
			container->Render();
		}
	}
}