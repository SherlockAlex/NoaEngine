#include "Shooter.h"
#include "MainMenuDelegate.h"
#include "GameDelegate.h"

Shooter::Shooter()
	:noa::NoaEngine(1920/2,1080/2,noa::WindowMode::WINDOW,"Shooter-2D")
{
	noa::Scene* mainMenu = noa::sceneManager.CreateScene("MainMenu");
	mainMenu->onLoad += MainMenuDelegate::OnLoad;

	noa::Scene* gameScene = noa::sceneManager.CreateScene("Game");
	gameScene->onLoad += GameDelegate::OnLoad;
	gameScene->onUpdate += GameDelegate::OnUpdate;

	noa::sceneManager.LoadScene("MainMenu");

}
