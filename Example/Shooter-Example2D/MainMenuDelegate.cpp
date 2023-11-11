#include "MainMenuDelegate.h"
#include "ResourceManager.h"

//���±�дUIϵͳ

void MainMenuDelegate::OnLoad(noa::Scene* scene)
{
	noa::UICanvasActor* canvas = noa::UICanvasActor::Create(scene);

	noa::UIGroup* mainGroup = noa::UIGroup::Create(canvas);

	noa::Image* backgroundImage = noa::Image::Create(mainGroup)
		->SetSprite(&ResourceManager::backgroundImage)
		.SetStyle(noa::ImageStyle::COVER)
		.Apply();

	noa::Button* startButton = noa::Button::Create(mainGroup)
		->SetText("Start")
		.SetFontSize(50)
		.SetSprite(&ResourceManager::defaultImage)
		.SetPosition(3*noa::Screen::width / 8, 3*noa::Screen::height / 8)
		.SetScale(noa::Screen::width / 8, noa::Screen::height / 8)
		.AddClickEvent(
			[]()
			{
				noa::sceneManager.LoadScene("Game");
			})
		.Apply();
}
