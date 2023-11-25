#include "MainMenuDelegate.h"
#include "ResourceManager.h"
#include "MainMenuUI.h"

void MainMenuDelegate::OnLoad(noa::Scene* scene)
{
	MainMenuUI* mainMenuUI = noa::NObject<MainMenuUI>::Create(scene);
}

void MainMenuDelegate::OnUpdate(noa::Scene* scene) 
{

}
