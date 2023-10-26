#ifdef _WIN64

#ifndef NOAENGINE_PLATFORM_WINDOWS_H
#define NOAENGINE_PLATFORM_WINDOWS_H

#include "Platform.h"

namespace noa {
	class Platform_Windows:public Platform
	{
	private:
		SDL_Window* window = nullptr;
		SDL_Event e = {};

	private:
		void UpdateInputSystem();

	public:

		Platform_Windows();
		~Platform_Windows();

		//��������
		int Create(int width, int height,
				WindowMode windowMode,
				std::string gameName) override;

		void EventLoop() override;

		//ȷ�����ڹر�
		bool CheckWindowClose() override;

	};
}

#endif // !NOAENGINE_PLATFORM_WINDOWS_H

#endif // _WIN64