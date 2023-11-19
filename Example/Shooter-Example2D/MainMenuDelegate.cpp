#include "MainMenuDelegate.h"
#include "ResourceManager.h"

void MainMenuDelegate::OnLoad(noa::Scene* scene)
{
	noa::UICanvasActor* canvas = noa::UICanvasActor::Create(scene);

	//创建一个UI容器
	noa::UIGroup* mainGroup = noa::UIGroup::Create(canvas)
		->SetPosition(noa::Screen::width/2,noa::Screen::height/2)
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
		.SetRadius(50)
		.SetNormalColor(noa::RGBA(255,255,255,200))
		.SetHeightLightColor(noa::RGBA(50,50,200,255))
		.SetTextClickColor(noa::BLACK)
		.SetAnchor(0.5f,0.5f)
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
		.SetPosition(0, 70)
		.Clone(startButton)
		.AddClickCallback(openSecondGroup)
		.Apply();

	noa::Button* quitButton = noa::Button::Create(mainGroup)
		->SetText(L"退出游戏")
		.SetPosition(0,2*70)
		.Clone(startButton)
		.AddClickCallback(noa::Application::Quit)
		.Apply();


	noa::Image* backgroundImage1 = noa::Image::Create(secondGroup)
		->SetSprite(&ResourceManager::backgroundImage1)
		.SetStyle(noa::ImageStyle::DEFAULT)
		.SetSize(noa::Screen::width/2,noa::Screen::height/2)
		.Apply();

	noa::Button* backButton = noa::Button::Create(secondGroup)
		->SetText(L"返回菜单")
		.SetPosition(0, 0)
		.Clone(startButton)
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
