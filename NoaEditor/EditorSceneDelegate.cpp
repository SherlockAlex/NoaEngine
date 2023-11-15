#include "EditorSceneDelegate.h"
#include "NoaEditor.h"
#include "EditorCamera.h"

noa::Sprite defaultBtn = noa::Sprite("btn-white.spr",64,64);

void noa::EditorSceneDelegate::OnLoad(noa::Scene* scene) 
{
	noa::UICanvasActor* canvas = noa::UICanvasActor::Create(scene);
	noa::UIGroup* group = noa::UIGroup::Create(canvas)
		->SetID("main_group")
		.SetPosition(100,0)
		.Apply();

	auto newProjectButtonClick = [](){
		noa::Canvas* canvas = noa::sceneManager.FindActorWithType<UICanvasActor>();
		if (!canvas) 
		{
			return;
		}
		canvas->OpenGroup("loader_group");

	};

	noa::Label* label = noa::Label::Create(group)
		->SetText(L"Hello World")
		.SetFontSize(15)
		.SetID("camera_info")
		.SetColor(noa::WHITE)
		.Apply();

	noa::Button* newProjectButton = noa::Button::Create(group)
		->SetID("project_btn")
		.SetText(L"新建工程")
		.SetSprite(&defaultBtn)
		.SetFontSize(15)
		.SetPosition(0, 60)
		.AddClickEvent(newProjectButtonClick)
		.Apply();

	noa::UIGroup* loaderGroup = noa::UIGroup::Create(canvas)
		->SetID("loader_group")
		.Apply();

	

	auto closeButtonClick = []() {
		noa::Canvas* canvas = noa::sceneManager.FindActorWithType<noa::UICanvasActor>();
		if (!canvas) 
		{
			return;
		}
		canvas->CloseGroup();
	};
	noa::Button* closeLoaderGroupButton = noa::Button::Create(loaderGroup)
		->SetSprite(&defaultBtn)
		.SetText(L"返回")
		.SetFontSize(15)
		.SetPosition(0,30)
		.AddClickEvent(closeButtonClick)
		.Apply();

	//添加场景编辑相机
	noa::EditorCamera* camera = noa::NObject<EditorCamera>::Create(scene)
		->SetTileScale(64,64)
		.Apply();

}

void noa::EditorSceneDelegate::OnUpdate(noa::Scene* scene)
{

	Vector<float> position;
	noa::EditorCamera* camera = dynamic_cast<noa::EditorCamera*>(noa::sceneManager.GetActiveScene()->GetMainCamera());
	if (camera) 
	{
		position = camera->transform.position;
	}

	noa::UICanvasActor* canvas = noa::sceneManager.FindActorWithType<noa::UICanvasActor>();
	if (!canvas) 
	{
		return;
	}
	noa::Label* label = canvas->GetLabelByID("camera_info");
	/*label->SetText(L"camera positon:("
		+std::to_wstring(position.x)
		+L","+std::to_wstring(position.y)+L")"
	);*/

	label->SetText(L"新建工程");

}