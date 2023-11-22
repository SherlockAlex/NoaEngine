#include "EditorSceneDelegate.h"
#include "NoaEditor.h"
#include "EditorCamera.h"

noa::Sprite defaultBtn = noa::Sprite("btn-white.spr",64,64);

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
		.SetPosition(noa::Screen::width/2,noa::Screen::height/2)
		.Apply();

	noa::UIContainer* subContainer =
		noa::UIContainer::Create(container)
		->SetID("warry_container")
		.SetPosition(0,0)
		.SetVisiable(true)
		.Apply();

	noa::Image* image =
		noa::Image::Create(subContainer)
		->SetID("editor_image")
		.SetColor(noa::RGBA(0,0,200,128))
		.SetSize(noa::Screen::width/2,noa::Screen::height/2)
		.SetAnchor(0.5f,0.0f)
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
		.SetPosition(-50,75)
		.SetAnchor(0.0f,0.5f)
		.Apply();

	noa::Label* processName =
		noa::Label::Create(subContainer)
		->SetFontSize(20)
		.SetID("process_name_label")
		.SetAnchor(0.0f,0.5f)
		.SetText(L"音量(100%):")
		.SetPosition(-170,72)
		.SetColor(noa::WHITE)
		.Apply();


	noa::Button* button =
		noa::Button::Create(subContainer)
		->SetID("button")
		.SetText(L"关闭")
		.SetSize(160,50)
		.SetRadius(50)
		.SetPosition(0,235)
		.AddClickCallback(CloseWindow)
		.Apply();

	noa::Label* label =
		noa::Label::Create(container)
		->SetFontSize(20)
		.SetID("camera_pos")
		.SetColor(noa::WHITE)
		.SetAnchor(0.0f,0.0f)
		.SetPosition(-noa::Screen::width/2,-noa::Screen::height/2)
		.Apply();

	noa::Label* deltaTime =
		noa::Label::Create(container)
		->SetFontSize(20)
		.SetID("deltaTime_label")
		.SetColor(noa::WHITE)
		.SetAnchor(0.0f, 0.0f)
		.SetPosition(-noa::Screen::width / 2, -noa::Screen::height / 2+25)
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

	noa::UIDocument* document = noa::UIHub::GetDocumentByID("editor_document");
	
	if (document)
	{
		GUIBehaviour(document);
	}

	

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

	noa::UIContainer* subContainer =
		document->GetElementByID<noa::UIContainer>("warry_container");
	if (subContainer)
	{
		noa::Vector<double> mousePos = 
			noa::Input::GetMousePosition();
		if (noa::Input::GetMouseKeyHold(noa::MouseButton::LEFT_BUTTON)) 
		{
			if (mousePos.y>=subContainer->globalTransform.position.y -10
				&&mousePos.y<=subContainer->globalTransform.position.y+10
				&&mousePos.x>=subContainer->globalTransform.position.x - 0.5f* noa::Screen::width / 3
				&&mousePos.y<=subContainer->globalTransform.position.x + 0.5f* noa::Screen::width / 3)
			{
				subContainer->SetGlobalPosition(mousePos.x,mousePos.y);
			}
		}
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