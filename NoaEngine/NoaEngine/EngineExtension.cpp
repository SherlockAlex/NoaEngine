#include "EngineExtension.h"

void noa::EnginExtension::Delete(EnginExtension*& ptr) {
	delete this;
	ptr = nullptr;
}