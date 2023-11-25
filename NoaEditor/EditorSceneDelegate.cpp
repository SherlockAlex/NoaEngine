#include "EditorSceneDelegate.h"
#include "NoaEditor.h"
#include "EditorCamera.h"
#include "EditorMainUI.h"


void noa::EditorSceneDelegate::OnLoad(noa::Scene* scene) 
{
	//��ӳ����༭���
	noa::EditorCamera* camera = noa::NObject<EditorCamera>::Create(scene)
		->SetFar(64)
		.Apply();
	noa::EditorMainUI* ui = 
		noa::NObject<noa::EditorMainUI>::Create(scene);
}

void noa::EditorSceneDelegate::OnTick(noa::Scene* scene)
{
	
}


