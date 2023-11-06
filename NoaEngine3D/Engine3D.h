#ifndef NOAENGINE3D_ENGINE3D_H
#define NOAENGINE3D_ENGINE3D_H

#include <functional>
#include <chrono>

#include "Debug.h"

namespace noa {

	namespace noa3d {
		class Engine3D
		{

		private:
			uint32_t windowID = 0;
			void* context = nullptr;

		public:
			
			std::function<void()> beginAction;
			std::function<void()> tickAction;

			Engine3D();
			~Engine3D();

			int Run();

		};
	}
}

#endif // !NOAENGINE3D_ENGINE3D_H



