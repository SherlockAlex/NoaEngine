#include "NoaEngine.h"

#if defined(_WIN64)||defined(_WIN32)||defined(__linux__)
int main(int argc, int argv)
{
	noa::NoaEngine* engine = noa::Application::CreateEngine();
	engine->Run();
	delete engine;
	return 0;
}
#endif // !
