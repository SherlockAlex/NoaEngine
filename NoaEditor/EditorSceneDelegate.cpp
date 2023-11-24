#include "EditorSceneDelegate.h"
#include "NoaEditor.h"
#include "EditorCamera.h"
#include "UIMove.h"

void GUIBehaviour(noa::UIDocument* document);

void CloseWindow();

void InitGUI(noa::Scene* scene)
{
	noa::UIDocument* document =
		noa::UIDocumentActor::Create(scene)
		->SetID("editor_document")
		.Apply();

	noa::UIContainer* container =
		noa::UIContainer::Create(document)
		->SetID("editor_container")
		.SetLocalPosition(noa::Screen::width/2,noa::Screen::height/2)
		.Apply();

	noa::UIContainer* subContainer =
		noa::UIContainer::Create(container)
		->SetID("warry_container")
		.SetVisiable(true)
		.SetLocalSize(noa::Screen::width / 2, 100)
		.SetLocalPosition(0,0)
		.SetAnchor(0.5f,0.5f)
		.Apply();
	UIMove* subContainerMove = noa::NObject<UIMove>::Create(subContainer);

	noa::Sprite sprite("background.spr", 1920, 1080);
	noa::Image* image =
		noa::Image::Create(subContainer)
		->SetID("editor_image")
		.SetColor(noa::RGBA(128,128,128,255))
		.SetSize(noa::Screen::width/2,noa::Screen::height/2)
		.SetRadius(25)
		.SetAnchor(0.5f,0.0f)
		.SetSprite(&sprite)
		.Apply();

	noa::Label* titleLabel =
		noa::Label::Create(subContainer)
		->SetFontSize(40)
		.SetID("title_label")
		.SetText(L"窗口")
		.SetAnchor(0.5f,0.0f)
		.SetColor(noa::WHITE)
		.Apply();

	noa::ProcessBar* processBar =
		noa::ProcessBar::Create(subContainer)
		->SetSize(240,10)
		.SetID("process_bar")
		.SetBackgroundColor(noa::RGBA(255,255,255,255))
		.SetFillColor(noa::RGBA(255,0,0,255))
		.SetLocalPosition(-50,75)
		.SetAnchor(0.0f,0.5f)
		.SetRadius(50)
		.Apply();

	noa::Label* processName =
		noa::Label::Create(subContainer)
		->SetFontSize(20)
		.SetID("process_name_label")
		.SetAnchor(0.0f,0.5f)
		.SetText(L"音量(100%):")
		.SetLocalPosition(-170,72)
		.SetColor(noa::WHITE)
		.Apply();

	noa::Label* text =
		noa::Label::Create(subContainer)
		->SetFontSize(20)
		.SetColor(noa::WHITE)
		.SetLocalPosition(0,100)
		.SetAnchor(0.5f,0.0f)
		.SetRowCount(6)
		.SetText(L"666666666666666666666666666666666666666")
		.Apply();


	noa::Button* button =
		noa::Button::Create(subContainer)
		->SetID("button")
		.SetText(L"关闭")
		.SetSize(160,50)
		.SetRadius(50)
		.SetLocalPosition(0,235)
		.AddClickCallback(CloseWindow)
		.Apply();

	noa::Label* label =
		noa::Label::Create(container)
		->SetFontSize(20)
		.SetID("camera_pos")
		.SetColor(noa::WHITE)
		.SetAnchor(0.0f,0.0f)
		.SetLocalPosition(-noa::Screen::width/2,-noa::Screen::height/2)
		.Apply();

	noa::Label* deltaTime =
		noa::Label::Create(container)
		->SetFontSize(20)
		.SetID("deltaTime_label")
		.SetColor(noa::WHITE)
		.SetAnchor(0.0f, 0.0f)
		.SetLocalPosition(-noa::Screen::width / 2, -noa::Screen::height / 2+25)
		.Apply();

	document->Display(container);

}

void noa::EditorSceneDelegate::OnLoad(noa::Scene* scene) 
{
	//添加场景编辑相机
	noa::EditorCamera* camera = noa::NObject<EditorCamera>::Create(scene)
		->SetFar(64)
		.Apply();

	InitGUI(scene);

}

void noa::EditorSceneDelegate::OnTick(noa::Scene* scene)
{

}

void GUIBehaviour(noa::UIDocument* document) {

	//屏幕信息显示

	noa::Vector<float> cameraPostion =
		noa::sceneManager.FindActorWithType<noa::EditorCamera>()->transform.position;

	std::wstring cameraInfo = L"camera position:"
		+ std::to_wstring(cameraPostion.x)
		+ L","
		+ std::to_wstring(cameraPostion.y);
	
	noa::Label* label =
		document->GetElementByID<noa::Label>("camera_pos");
	if (label)
	{
		label->SetText(cameraInfo);
	}

	noa::Label* deltaTimeLabel =
		document->GetElementByID<noa::Label>("deltaTime_label");
	if (deltaTimeLabel) {
		deltaTimeLabel->SetText(L"fps:"
			+ std::to_wstring(1.0f / noa::Time::realDeltaTime));
	}

	noa::ProcessBar* processBar =
		document->GetElementByID<noa::ProcessBar>("process_bar");
	noa::Label* processName =
		document->GetElementByID<noa::Label>("process_name_label");
	const std::wstring processInfo = L"音量("
		+ std::to_wstring(
			static_cast<int>(processBar->GetValue() * 100.0f)
		)
		+ L"%):";
	processName->SetText(processInfo);
}

void CloseWindow() {
	noa::UIDocument* document =
		noa::UIHub::GetDocumentByID("editor_document");
	noa::UIContainer* container =
		document->GetElementByID<noa::UIContainer>("warry_container");
	container->SetVisiable(false);
	noa::Debug::Log("close window");
}