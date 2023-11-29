#include "Shooter.h"
#include "MainMenuDelegate.h"
#include "GameDelegate.h"

Shooter::Shooter()
	:noa::NoaEngine(1920/2,1080/2
		,noa::WindowMode::FULLSCREEN
		,noa::GraphicsAPI::OPENGL
		,"Shooter-2D"
	)
{
	noa::Scene* mainMenu = noa::Scene::Create("MainMenu");
	mainMenu->onLoad += MainMenuDelegate::OnLoad;
	mainMenu->onUpdate += MainMenuDelegate::OnUpdate;

	noa::Scene* gameScene = noa::Scene::Create("Game");
	gameScene->onLoad += GameDelegate::OnLoad;
	gameScene->onTick += GameDelegate::OnTick;

	noa::SceneManager::LoadScene("MainMenu");
}
