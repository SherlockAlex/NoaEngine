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
		->SetText(L"开始游戏")
		.SetFontSize(20)
		.SetPosition(3 * noa::Screen::width / 8, 3 * noa::Screen::height / 8)
		.SetRadius(25)
		.SetSize(160, 60)
		.SetNormalColor(noa::WHITE)
		.SetHeightLightColor(noa::BLUE)
		.SetTextClickColor(noa::BLACK)
		.SetTextHeightLightColor(noa::WHITE)
		.SetNormalScale(1.0f)
		.SetHeightLightScale(1.1f)
		.SetClickScale(0.9f)
		.AddClickCallback(
			[]()
			{
				noa::sceneManager.LoadScene("Game");
			})
		.Apply();

	noa::Button* quitButton = noa::Button::Create(mainGroup)
		->SetText(L"退出游戏")
		.SetPosition(3 * noa::Screen::width / 8, 3 * noa::Screen::height / 8 + 70)
		.Clone(startButton)
		.AddClickCallback(
			[]()
			{
				
			})
		.Apply();
}
