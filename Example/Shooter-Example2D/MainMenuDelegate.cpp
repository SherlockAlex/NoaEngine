#include "MainMenuDelegate.h"
#include "ResourceManager.h"

void MainMenuDelegate::OnLoad(noa::Scene* scene)
{
	noa::UICanvasActor* canvas = noa::UICanvasActor::Create(scene);

	noa::UIGroup* mainGroup = noa::UIGroup::Create(canvas);

	noa::Image* backgroundImage = noa::Image::Create(mainGroup)
		->SetSprite(&ResourceManager::backgroundImage)
		.SetStyle(noa::ImageStyle::COVER)
		.Apply();

	noa::Button* startButton = noa::Button::Create(mainGroup)
		->SetText(L"¿ªÊ¼ÓÎÏ·")
		.SetFontSize(50)
		.SetSprite(&ResourceManager::defaultImage)
		.SetPosition(3*noa::Screen::width / 8, 3*noa::Screen::height / 8)
		.SetScale(noa::Screen::width / 4, noa::Screen::height / 4)
		.AddClickEvent(
			[]()
			{
				noa::sceneManager.LoadScene("Game");
			})
		.Apply();
}
