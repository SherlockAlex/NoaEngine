#ifdef _WIN64

#ifndef NOAENGINE_PLATFORM_WINDOWS_H
#define NOAENGINE_PLATFORM_WINDOWS_H

#include "Platform.h"

namespace noa {
	class Platform_Windows:public Platform
	{
	public:

		Platform_Windows();

		//��������
		int Create(int width, int height,
				WINDOWMODE windowMode,
				std::string gameName) override;

		//ȷ�����ڹر�
		bool CheckWindowClose() override;

		int SystemLoop() override;

	};
}

#endif // !NOAENGINE_PLATFORM_WINDOWS_H

#endif // _WIN64