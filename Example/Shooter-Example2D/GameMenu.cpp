#include "GameMenu.h"

void ReturnMainMenu();

GameMenu::GameMenu(noa::Scene* scene):noa::Actor(scene)
{
	
}

void GameMenu::Start() {
	
	noa::Image* background =
		noa::Image::Create(container)
		->SetID("game_memu_background")
		.SetStyle(noa::ImageStyle::COVER)
		.SetColor(noa::RGBA(0,0,0,230))
		.Apply();

	noa::Image* uihub =
		noa::Image::Create(container)
		->SetID("game_memu_uihub")
		.SetStyle(noa::ImageStyle::DEFAULT)
		.SetSize(noa::Screen::width/2, noa::Screen::height / 1.5f)
		.SetAnchor(0.5f,0.5f)
		.SetRadius(30)
		.SetColor(noa::RGBA(0, 0, 0, 250))
		.Apply();

	noa::Label* title =
		noa::Label::Create(container)
		->SetID("game_memu_title")
		.SetFontSize(50)
		.SetAnchor(0.5f, 0.5f)
		.SetColor(noa::WHITE)
		.SetText(L"游戏菜单")
		.SetLocalPosition(0,-150)
		.Apply();

	noa::Button* button =
		noa::Button::Create(container)
		->SetID("game_menu_return_main_menu_button")
		.SetRadius(50)
		.AddClickCallback(ReturnMainMenu)
		.SetText(L"返回主菜单")
		.SetAnchor(0.5f,0.5f)
		.Apply();

}

void GameMenu::Show() {
	document->Display(container);
}

void GameMenu::Close() {
	document->Close();
}


void ReturnMainMenu() 
{
	noa::SceneManager::LoadScene("MainMenu");
}