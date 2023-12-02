#include "MainMenuUI.h"

MainMenuUI::MainMenuUI(noa::Scene* scene)
	:noa::Actor(scene)
{
	CreateMainContainer();
	document->Display("main_container");
}

MainMenuUI::~MainMenuUI() {

}

void MainMenuUI::CreateMainContainer() {
	noa::UIContainer* container
		= noa::UIContainer::Create(document)
		->SetID("main_container")
		.SetLocalPosition(0, 0)
		.SetAnchor(0.5f, 0.5f)
		.SetLocalSize(noa::Screen::width, noa::Screen::height)
		.Apply();

	noa::Button* button
		= noa::Button::Create(container)
		->SetID("main_button")
		.AddClickCallback([this]() {noa::Debug::Log("button click"); })
		.Apply();

}