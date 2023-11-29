#include "EditorWindow.h"
#include "Editor.h"

noa::EditorWindow::EditorWindow()
{
	noa::Editor::AddEditorWindow(this);
}

noa::EditorWindow::~EditorWindow() {

}

void noa::EditorWindow::Delete(noa::EditorWindow*& ptr)
{
	delete this;
	ptr = nullptr;
}

void noa::EditorWindow::SetName(const std::string& name)
{
	this->name = name;
}