#pragma once
#include "NoaEditor.h"

namespace noa {
	class EditorMainUI :public noa::Actor
	{
	private:
		ACTOR(EditorMainUI)
	private:
		EditorMainUI(noa::Scene* scene);

		void Start() override;
		noa::UIContainer* CreateMainUI();
		noa::UIContainer* CreateTestWindow(UIContainer* container);

	private:
		noa::UIDocument* document =
			noa::UIDocumentComponent::Create(this)
			->SetID("editor_main_document")
			.Apply();
	};

}


