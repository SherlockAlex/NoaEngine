#include "Application.h"
#include "Platform.h"

void noa::Application::Quit() {
	platform->Quit();
}

