#include "Editor.h"
#include "ImGuiHelper.h"

#include "NoaEditor.h"
#include "EditorWindow.h"

#include "PropertyWindow.h"

SDL_Event* noa::Editor::ioEvent = nullptr;

std::vector<noa::EditorWindow*> noa::Editor::windows;

void noa::Editor::Start()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	
	const uint32_t windowID = noa::platform->GetWindowID();
	SDL_Window* window = SDL_GetWindowFromID(windowID);
	void* context = noa::renderer->GetContext();
	ImGui_ImplSDL2_InitForOpenGL(window,context);

	ImGui_ImplOpenGL3_Init("#version 330");

	ioEvent =(SDL_Event*)(noa::platform->GetPlatformEvent()
			->GetEventEntity());

	noa::platform->GetPlatformEvent()->AddPollEventCallback(
		[]() {ImGui_ImplSDL2_ProcessEvent(ioEvent);});

	ImGui::StyleColorsLight();

	noa::PropertyWindow* propertyWindow
		= noa::NObject<noa::PropertyWindow>::Create();

}

void noa::Editor::BeforeUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

static bool showWindow = true;
void noa::Editor::Update() 
{

	for (auto& window:windows)
	{
		if (window)
		{
			ImGui::Begin(window->name.c_str());
			window->OnGUI();
			ImGui::End();
		}
	}
}

void noa::Editor::Render() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void noa::Editor::OnExit() {

	for (auto& window:windows)
	{
		if (window)
		{
			window->Delete(window);
		}
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void noa::Editor::AddEditorWindow(noa::EditorWindow* window) 
{
	if (!window) 
	{
		return;
	}
	windows.push_back(window);
}