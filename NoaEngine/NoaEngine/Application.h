#ifndef NOAENGINE_APPLICATION_H
#define NOAENGINE_APPLICATION_H

namespace noa {
	class NoaEngine;
	class Application {
	public:
		static void Quit();
		static NoaEngine* CreateEngine();
	};
}

#endif // !NOAENGINE_APPLICATION_H
