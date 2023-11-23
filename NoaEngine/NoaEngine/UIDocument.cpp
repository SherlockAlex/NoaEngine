#include "UIDocument.h"
#include "UIContainer.h"
#include "UIHub.h"

noa::UIDocument::UIDocument() {
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

void noa::UIDocument::SetDocumentID(const std::string& id)
{
	this->id = id;
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

void noa::UIDocument::UIDocumentStart() {

}

void noa::UIDocument::UIDocumentUpdate()
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

void noa::UIDocument::UIDocumentRender() {
	for (auto& container : containerList)
	{
		//越往前的越先绘制
		if (container && container->visiable)
		{
			container->Render();
		}
	}
}


noa::UIDocumentActor::UIDocumentActor(Scene* scene) :Actor(scene), UIDocument()
{

}

noa::UIDocumentActor::~UIDocumentActor()
{

}

noa::UIDocumentActor* noa::UIDocumentActor::Create(Scene* scene)
{
	return noa::NObject<UIDocumentActor>::Create(scene);
}

void noa::UIDocumentActor::Start()
{
	UIDocument::UIDocumentStart();
}

void noa::UIDocumentActor::Update()
{
	UIDocument::UIDocumentUpdate();
}

void noa::UIDocumentActor::Render() {
	UIDocument::UIDocumentRender();
}

noa::UIDocumentActor& noa::UIDocumentActor::SetActorTag(
	const std::string& tag)
{
	this->tag = tag;
	return *this;
}

noa::UIDocumentActor& noa::UIDocumentActor::SetID(
	const std::string& id)
{
	SetDocumentID(id);
	return *this;
}

noa::UIDocumentActor* noa::UIDocumentActor::Apply() {
	return this;
}

noa::UIDocumentComponent::UIDocumentComponent(Actor* actor) :ActorComponent(actor)
{

}

noa::UIDocumentComponent::~UIDocumentComponent() {

}

noa::UIDocumentComponent* noa::UIDocumentComponent::Create(Actor* actor)
{
	return noa::NObject<UIDocumentComponent>::Create(actor);
}

noa::UIDocumentComponent& noa::UIDocumentComponent::SetID(
	const std::string& id)
{
	noa::UIDocument::SetDocumentID(id);
	return *this;
}

noa::UIDocumentComponent* noa::UIDocumentComponent::Apply() {
	return this;
}

void noa::UIDocumentComponent::Start() {
	UIDocument::UIDocumentStart();
}

void noa::UIDocumentComponent::Update() {
	UIDocument::UIDocumentUpdate();
}

void noa::UIDocumentComponent::Render() {
	UIDocument::UIDocumentRender();
}