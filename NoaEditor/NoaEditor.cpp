#include "NoaEditor.h"
#include "EditorSceneDelegate.h"

noa::NoaEditor::NoaEditor() 
	:noa::NoaEngine(1920/2,1080/2,WindowMode::WINDOW,GraphicsAPI::OPENGL,"NoaEditor")
{
	noa::Scene* editorScene = noa::sceneManager.CreateScene("EditorScene");
	editorScene->onLoad += EditorSceneDelegate::OnLoad;
	editorScene->onTick += EditorSceneDelegate::OnTick;

	noa::sceneManager.LoadScene("EditorScene");

}

noa::NoaEditor::~NoaEditor() {

}

int main() 
{
	noa::NoaEditor editor;
	editor.Run();
	return 0;
}