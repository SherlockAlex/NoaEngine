#include "GameUI.h"

GameUI::GameUI(noa::Scene* scene)
	:noa::Actor(scene)
{

}

void GameUI::Start()
{
	//³õÊ¼»¯
	noa::UIContainer* container =
		noa::UIContainer::Create(document)
		->SetID("game_ui_container")
		.SetAnchor(0,0)
		.Apply();

	noa::ScrollBar* hpBar =
		noa::ScrollBar::Create(container)
		->SetID("game_ui_hpbar")
		.SetLocalSize(240,25)
		.SetAnchor(0.0f,0.0f)
		.SetLocalPosition(5,5)
		.Apply();

	document->Display(container);

}