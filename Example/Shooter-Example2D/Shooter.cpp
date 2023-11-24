#include "Shooter.h"
#include "MainMenuDelegate.h"
#include "GameDelegate.h"

Shooter::Shooter()
	:noa::NoaEngine(1920/2,1080/2
		,noa::WindowMode::WINDOW
		,noa::GraphicsAPI::OPENGL
		,"Shooter-2D"
	)
{
	noa::Scene* mainMenu = noa::sceneManager.CreateScene("MainMenu");
	mainMenu->onLoad += MainMenuDelegate::OnLoad;
	mainMenu->onUpdate += MainMenuDelegate::OnUpdate;

	noa::Scene* gameScene = noa::sceneManager.CreateScene("Game");
	gameScene->onLoad += GameDelegate::OnLoad;
	gameScene->onTick += GameDelegate::OnTick;

	noa::sceneManager.LoadScene("MainMenu");
}
