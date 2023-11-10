#include "MainMenuDelegate.h"
#include "ResourceManager.h"

//从新编写UI系统

void MainMenuDelegate::OnLoad(noa::Scene* scene)
{
	noa::UICanvas* canvas = noa::UICanvas::Create(scene);
	
	noa::Image* backgroundImage = noa::Image::Create(canvas)
		->SetSprite(&ResourceManager::backgroundImage)
		.SetStyle(noa::ImageStyle::COVER)
		.Apply();

	noa::Button* startButton = noa::Button::Create(canvas)
		->SetText("START")
		.SetFontSize(10)
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
