#include "MainMenuDelegate.h"
#include "ResourceManager.h"

void LoadGameButtonClick() {
	noa::sceneManager.LoadScene("Game");
}

void BackButtonClick() {
	noa::UIDocument* document = 
		noa::sceneManager.FindActorWithType<noa::UIDocumentActor>();

	if (!document) 
	{
		return;
	}
	document->Close();
}

void ShowSecondContainerButtonClick() {
	noa::UIDocument* document =
		noa::sceneManager.FindActorWithType<noa::UIDocumentActor>();
	if (!document) {
		return;
	}
	document->Display(1);
}

void ProcessBarFinishedAction() {
	noa::Debug::Log("this process is finished");
}

void MainMenuDelegate::OnLoad(noa::Scene* scene)
{
	noa::UIDocument* document = noa::UIDocumentActor::Create(scene)
		->SetID("main_menu_document")
		.Apply();

	noa::UIContainer* mainContainer = noa::UIContainer::Create(document)
		->SetLocalPosition(noa::Screen::width,noa::Screen::height/2)
		.SetID("main_container")
		.Apply();

	document->Display(mainContainer);

	noa::UIContainer* secondContainer = noa::UIContainer::Create(document)
		->SetLocalPosition(noa::Screen::width / 2, noa::Screen::height / 2)
		.SetID("second_container")
		.Apply();

	noa::Image* backgroundImage = noa::Image::Create(mainContainer)
		->SetSize(1920,1080)
		.SetSprite(&ResourceManager::backgroundImage)
		.SetStyle(noa::ImageStyle::COVER)
		.Apply();

	noa::Button* startButton = noa::Button::Create(mainContainer)
		->SetText(L"开始游戏")
		.SetFontSize(20)
		.SetLocalPosition(0,0)
		.SetSize(240, 60)
		.SetRadius(0)
		.SetNormalColor(noa::RGBA(20,20,20,250))
		.SetHeightLightColor(noa::RGBA(255,0,0,255))
		.SetAnchor(1.0f,0.5f)
		.SetTextOffset(-0.15f,0.0f)
		.SetTextNormalColor(noa::WHITE)
		.SetTextHeightLightColor(noa::WHITE)
		.SetTextClickColor(noa::BLACK)
		.SetNormalScale(1.0f)
		.SetHeightLightScale(1.1f)
		.SetClickScale(0.9f)
		.SetSelectedAudio(noa::Resource::LoadAudioClip("buttonselect.mp3"))
		.AddClickCallback(LoadGameButtonClick)
		.Apply();

	noa::Button* openButton = noa::Button::Create(mainContainer)
		->SetText(L"游戏设置")
		.Clone(startButton)
		.SetLocalPosition(0,70)
		.AddClickCallback(ShowSecondContainerButtonClick)
		.Apply();

	noa::Button* quitButton = noa::Button::Create(mainContainer)
		->SetText(L"退出游戏")
		.Clone(startButton)
		.SetLocalPosition(0,2*70)
		.AddClickCallback(noa::Application::Quit)
		.Apply();

	noa::Image* backgroundImage1 = noa::Image::Create(secondContainer)
		->SetSize(noa::Screen::width / 1.5, noa::Screen::height / 1.5)
		.SetSprite(&ResourceManager::backgroundImage1)
		.SetStyle(noa::ImageStyle::DEFAULT)
		.Apply();

	noa::Button* backButton = noa::Button::Create(secondContainer)
		->SetText(L"返回菜单")
		.Clone(startButton)
		.SetLocalPosition(0, -200)
		.SetAnchor(0.5f,0.5f)
		.SetRadius(50)
		.SetTextOffset(0.0f,0.0f)
		.AddClickCallback(BackButtonClick)
		.Apply();

	noa::ScrollBar* processBar = noa::ScrollBar::Create(secondContainer)
		->SetSize(360,10)
		.SetID("process_bar")
		.SetAmount(0.5f)
		.SetBackgroundColor(noa::RGBA(20,20,20,250))
		.SetFillColor(noa::RGBA(255,255,255,255))
		.SetRadius(50)
		.SetInteractable(true)
		.SetFinishedCallback(ProcessBarFinishedAction)
		.SetActiveScale(2.0f)
		.Apply();
}

void MainMenuDelegate::OnUpdate(noa::Scene* scene) 
{

}
