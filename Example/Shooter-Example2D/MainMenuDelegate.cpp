#include "MainMenuDelegate.h"
#include "ResourceManager.h"

void MainMenuDelegate::OnLoad(noa::Scene* scene)
{
	noa::UIDocument* document = noa::UIDocumentActor::Create(scene);

	noa::UIContainer* mainContainer = noa::UIContainer::Create(document)
		->SetPosition(noa::Screen::width,noa::Screen::height/2)
		.SetID("main_container")
		.Apply();

	noa::UIContainer* secondContainer = noa::UIContainer::Create(document)
		->SetPosition(noa::Screen::width / 2, noa::Screen::height / 2)
		.SetID("second_container")
		.Apply();

	noa::Image* backgroundImage = noa::Image::Create(mainContainer)
		->SetSprite(&ResourceManager::backgroundImage)
		.SetStyle(noa::ImageStyle::COVER)
		.Apply();

	noa::Button* startButton = noa::Button::Create(mainContainer)
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
		noa::UIDocument* document =
			noa::sceneManager.FindActorWithType<noa::UIDocumentActor>();

		if (!document) {
			return;
		}

		document->Display("second_container");
	};
	noa::Button* openButton = noa::Button::Create(mainContainer)
		->SetText(L"游戏设置")
		.Clone(startButton)
		.SetPosition(0, 70)
		.AddClickCallback(openSecondGroup)
		.Apply();

	noa::Button* quitButton = noa::Button::Create(mainContainer)
		->SetText(L"退出游戏")
		.Clone(startButton)
		.SetPosition(0,2*70)
		.AddClickCallback(noa::Application::Quit)
		.Apply();


	noa::Image* backgroundImage1 = noa::Image::Create(secondContainer)
		->SetSprite(&ResourceManager::backgroundImage1)
		.SetStyle(noa::ImageStyle::DEFAULT)
		.SetSize(noa::Screen::width/1.5,noa::Screen::height/1.5)
		.Apply();

	noa::Button* backButton = noa::Button::Create(secondContainer)
		->SetText(L"返回菜单")
		.Clone(startButton)
		.SetPosition(0, -200)
		.SetAnchor(0.5f,0.5f)
		.SetRadius(50)
		.SetTextOffset(0.0f,0.0f)
		.AddClickCallback([]() {
			noa::UIDocument* document =
				noa::sceneManager.FindActorWithType<noa::UIDocumentActor>();

			if (!document) {
				return;
			}

			document->Close();

		})
		.Apply();


}
