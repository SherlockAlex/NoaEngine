#ifndef NOAENGINE_PLATFORM
#define NOAENGINE_PLATFORM


#include <string>
#include <functional>
#include <chrono>

#include "SDLHelper.h"

#ifdef _WIN64

#include "GLTexture.h"
#include "GLRenderer.h"

#endif // _WIN64



namespace noa {

	//��������
	enum class WindowMode
	{
		FULLSCREEN = 1,
		WINDOW = 2048
	};

	class InputEvent;
	//ƽ̨������
	class Platform {
	protected:
		//ϵͳʱ��

		uint32_t windowID = 0;

		bool isRun = true;

		std::shared_ptr<InputEvent> e = nullptr;

	public:

		Platform();
		virtual ~Platform();

		//��������
		virtual int Create(int width, int height,
				WindowMode windowMode,
				std::string gameName) = 0;

		std::shared_ptr<InputEvent> GetPlatformEvent();

		//ȷ�����ڹر�
		virtual bool CheckWindowClose() = 0;

		virtual void EventLoop() = 0;

		void Quit();

		uint32_t GetWindowID();


	};

	enum class KeyCode; // ǰ������
	struct MouseContext;
	// ƽ̨�����¼���ĳ���
	class InputEvent
	{
	private:
		friend class InputSystem;
	protected:
		std::shared_ptr<MouseContext> mouseContext = nullptr;
	public:
		InputEvent();
		virtual ~InputEvent();

		virtual bool GetKeyHold(noa::KeyCode key) = 0;
		virtual bool GetKeyDown(noa::KeyCode key) = 0;

		virtual void PollEvent(const std::function<void()> & quitCallback) = 0;
		virtual void Update() = 0;
		virtual void SetRelativeMouseMode(bool mode) = 0;
	};

}


#endif

