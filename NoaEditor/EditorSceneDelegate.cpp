#include "EditorSceneDelegate.h"
#include "NoaEditor.h"
#include "EditorCamera.h"

noa::Sprite defaultBtn = noa::Sprite("btn-white.spr",64,64);

void GUIBehaviour(noa::UIDocument* document);

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

	noa::Image* image =
		noa::Image::Create(container)
		->SetID("editor_image")
		.SetColor(noa::RGBA(128,128,128,128))
		.SetSize(noa::Screen::width/3,noa::Screen::height/3)
		.SetAnchor(0.5f,0.0f)
		.Apply();

	noa::Button* button =
		noa::Button::Create(container)
		->SetID("button")
		.SetSize(160,50)
		.SetRadius(50)
		.SetPosition(0,50)
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

	noa::UIContainer* container =
		document->GetElementByID<noa::UIContainer>("editor_container");

	noa::Vector<double> mousePos = noa::Input::GetMousePosition();
	if (noa::Input::GetMouseKeyHold(noa::MouseButton::LEFT_BUTTON))
	{
		if (mousePos.y>=container->globalTransform.position.y-10
			||mousePos.y<=container->globalTransform.position.y+10) 
		{
			container->SetPosition(mousePos.x,mousePos.y);
		}
	}

}