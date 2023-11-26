#include "EditorGUI.h"
#include "./../../imgui/imgui.h"
#include "./../../imgui/backends/imgui_impl_sdl2.h"
#include "./../../imgui/backends/imgui_impl_opengl3.h"

#include "NoaEditor.h"

SDL_Event* noa::EditorGUI::ioEvent = nullptr;

void noa::EditorGUI::Start() 
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	
	uint32_t windowID = noa::platform->GetWindowID();
	SDL_Window* window = SDL_GetWindowFromID(windowID);
	void* context = noa::renderer->GetContext();
	ImGui_ImplSDL2_InitForOpenGL(window,context);

	ImGui_ImplOpenGL3_Init("#version 330");

	ioEvent =(SDL_Event*)(noa::platform->GetPlatformEvent()
			->GetEventEntity());

	noa::renderer->SetRenderable(true);

	noa::platform->GetPlatformEvent()->AddPollEventCallback([]() {ImGui_ImplSDL2_ProcessEvent(ioEvent);});
}

void noa::EditorGUI::BeforeUpdate() 
{

	//Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	ImGui::ShowDemoWindow();

}

static bool showWindow = true;
void noa::EditorGUI::Update() {
	ImGui::ShowDemoWindow(&showWindow);
	ImGui::Begin("Hello, world!");

	//»æÖÆ±à¼­Æ÷UI
	ImGui::Text("hello world %d", 123);
	
	ImGui::End();
}

void noa::EditorGUI::Render() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void noa::EditorGUI::OnExit() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}