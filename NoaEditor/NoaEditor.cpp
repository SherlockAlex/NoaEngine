#include "NoaEditor.h"
#include "EditorSceneDelegate.h"
#include "Editor.h"

noa::NoaEditor::NoaEditor() 
	:noa::NoaEngine(1920/2,1080/2,WindowMode::WINDOW,GraphicsAPI::OPENGL,"NoaEditor")
{
	noa::Scene* editorScene = noa::SceneManager::CreateScene("EditorScene");
	editorScene->onLoad += EditorSceneDelegate::OnLoad;
	editorScene->onTick += EditorSceneDelegate::OnTick;

	noa::SceneManager::LoadScene("EditorScene");
}

noa::NoaEditor::~NoaEditor() {

}

void noa::NoaEditor::Start() {
	noa::Editor::Start();
}

void noa::NoaEditor::BeforeUpdate() {
	noa::Editor::BeforeUpdate();
}

void noa::NoaEditor::Update() {
	noa::Editor::Update();
}

void noa::NoaEditor::Render() {
	noa::Editor::Render();
}

void noa::NoaEditor::OnExit() {
	noa::Editor::OnExit();
}

int main() 
{
	noa::NoaEditor editor;
	editor.Run();
	return 0;
}