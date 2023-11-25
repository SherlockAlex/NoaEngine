#include "EditorMainUI.h"
#include "UIMove.h"

noa::EditorMainUI::EditorMainUI(noa::Scene* scene)
	:noa::Actor(scene)
{

}

void noa::EditorMainUI::Start() {
	noa::UIContainer* mainContainer = CreateMainUI();
	document->Display(mainContainer);
}

noa::UIContainer* noa::EditorMainUI::CreateMainUI() {
	noa::UIContainer* container =
		noa::UIContainer::Create(document)
		->SetID("main_container")
		.SetLocalPosition(0, 0)
		.SetLocalSize(noa::Screen::width, noa::Screen::height)
		.SetAnchor(0.5f, 0.5f)
		.SetVisiable(true)
		.Apply();

	noa::Image* image =
		noa::Image::Create(container)
		->SetID("window_background_image")
		.SetLocalPosition(0, 0)
		.SetSize(100, 100)
		.SetAnchor(1.0f,0.0f)
		.Apply();
	UIMove* move = noa::NObject<UIMove>::Create(image);

	return container;

}

noa::UIContainer* noa::EditorMainUI::CreateTestWindow(
	noa::UIContainer* father) 
{
	//创建一个window
	noa::UIContainer* container = 
		noa::UIContainer::Create(father)
		->SetVisiable(true)
		.SetID("window_container")
		.Apply();
}