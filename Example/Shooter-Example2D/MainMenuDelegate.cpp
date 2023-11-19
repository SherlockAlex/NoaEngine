#include "MainMenuDelegate.h"
#include "ResourceManager.h"

void MainMenuDelegate::OnLoad(noa::Scene* scene)
{
	noa::UICanvasActor* canvas = noa::UICanvasActor::Create(scene);

	//创建一个UI容器
	noa::UIGroup* mainGroup = noa::UIGroup::Create(canvas)
		->SetPosition(noa::Screen::width,noa::Screen::height/2)
		.SetID("main_group")
		.Apply();

	noa::UIGroup* secondGroup = noa::UIGroup::Create(canvas)
		->SetPosition(noa::Screen::width / 2, noa::Screen::height / 2)
		.SetID("second_group")
		.Apply();

	noa::Image* backgroundImage = noa::Image::Create(mainGroup)
		->SetSprite(&ResourceManager::backgroundImage)
		.SetStyle(noa::ImageStyle::COVER)
		.Apply();

	noa::Button* startButton = noa::Button::Create(mainGroup)
		->SetText(L"开始游戏")
		.SetFontSize(20)
		.SetPosition(0,0)
		.SetSize(240, 60)
		.SetRadius(0)
		.SetNormalColor(noa::RGBA(255,255,255,0))
		.SetHeightLightColor(noa::RGBA(255,0,0,255))
		.SetAnchor(1.0f,0.5f)
		.SetTextOffset(-0.15f,0.0f)
		.SetTextNormalColor(noa::WHITE)
		.SetTextHeightLightColor(noa::WHITE)
		.SetTextClickColor(noa::BLACK)
		.SetNormalScale(1.0f)
		.SetHeightLightScale(1.1f)
		.SetClickScale(0.9f)
		.AddClickCallback(
			[]()
			{
				noa::sceneManager.LoadScene("Game");
			})
		.Apply();

	auto openSecondGroup = []() {
		noa::Canvas* canvasActor =
			noa::sceneManager.FindActorWithType<noa::UICanvasActor>();

		if (!canvasActor) {
			return;
		}

		canvasActor->OpenGroup(1);
	};
	noa::Button* openButton = noa::Button::Create(mainGroup)
		->SetText(L"游戏设置")
		.Clone(startButton)
		.SetPosition(0, 70)
		.AddClickCallback(openSecondGroup)
		.Apply();

	noa::Button* quitButton = noa::Button::Create(mainGroup)
		->SetText(L"退出游戏")
		.Clone(startButton)
		.SetPosition(0,2*70)
		.AddClickCallback(noa::Application::Quit)
		.Apply();


	noa::Image* backgroundImage1 = noa::Image::Create(secondGroup)
		->SetSprite(&ResourceManager::backgroundImage1)
		.SetStyle(noa::ImageStyle::DEFAULT)
		.SetSize(noa::Screen::width/2,noa::Screen::height/2)
		.Apply();

	noa::Button* backButton = noa::Button::Create(secondGroup)
		->SetText(L"返回菜单")
		.Clone(startButton)
		.SetPosition(0, 0)
		.AddClickCallback([]() {
			noa::Canvas* canvasActor =
				noa::sceneManager.FindActorWithType<noa::UICanvasActor>();

			if (!canvasActor) {
				return;
			}

			canvasActor->CloseGroup();

		})
		.Apply();


}
