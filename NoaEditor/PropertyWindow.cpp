#include "PropertyWindow.h"

noa::PropertyWindow::PropertyWindow() :noa::EditorWindow()
{
	this->SetName("UI Window");
}

noa::PropertyWindow::~PropertyWindow() {

}

char text[50];
int size[2];
void noa::PropertyWindow::OnGUI() {
	//»æÖÆÃæ°åÊôÐÔ
	
	ImGui::DragInt2("position",position);
	ImGui::DragInt2("size",size);

	ImGui::BeginTabItem("scroll bar");
	ImGui::InputText("name", text, 50);
	ImGui::Text("hello world");
	ImGui::EndTabItem();

	noa::UIDocument* document 
		= noa::UIHub::GetDocumentByID("editor_main_document");
	if (document) 
	{
		noa::ScrollBar* bar 
			= document->GetElementByID<noa::ScrollBar>("window_background_scrollbar");
		
		bar->SetLocalPosition(position[0],position[1]);
		bar->SetLocalSize(size[0],size[1]);
	}
}

noa::Vector<int> noa::PropertyWindow::GetPositionValue() {
	noa::Vector<int> result;
	result.x = position[0];
	result.y = position[1];
	return result;
}