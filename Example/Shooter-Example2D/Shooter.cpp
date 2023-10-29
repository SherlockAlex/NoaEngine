#include "Shooter.h"
#include "MainMenu.h"

Shooter::Shooter()
	:noa::NoaEngine(1920/2,1080/2,noa::WindowMode::WINDOW,"Shooter-2D")
{
	noa::Scene* mainMenu = noa::sceneManager.CreateScene("MainMenu");
	mainMenu->onLoad += MainMenuDelegate::OnLoad;

	noa::sceneManager.LoadScene("MainMenu");

}
