#include "NoaEditor.h"
#include "ImGuiHelper.h"

void noa::NoaEditor::Start() 
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();
	
	SDL_Window* window = SDL_GetWindowFromID(noa::platform->GetWindowID());
	void* context = noa::renderer->GetContext();
	ImGui_ImplSDL2_InitForOpenGL(window,context);
	ImGui_ImplOpenGL3_Init("#version 330");

	eventPtr = noa::platform->GetPlatformEvent()->GetEventEntity();

	noa::platform->GetPlatformEvent()->AddPollEventCallback([this]() {ImGui_ImplSDL2_ProcessEvent((SDL_Event*)this->eventPtr)});

}
void noa::NoaEditor::BeforeUpdate()
{
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}
void noa::NoaEditor::Update()
{

}
void noa::NoaEditor::Render()
{

}
void noa::NoaEditor::OnExit()
{

}