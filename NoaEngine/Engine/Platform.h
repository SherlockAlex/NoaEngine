#ifndef NOAENGINE_PLATFORM
#define NOAENGINE_PLATFORM


#include <string>
#include <functional>
#include <chrono>
#include <memory>

#include "NoaAction.h"
#include "NoaMath.h"
#include "Core.h"

namespace noa {

	//��������
	enum class NOA_API WindowMode
	{
		
		FULLSCREEN = 4097,
		WINDOW = 2048
	};

	class NOA_API InputEvent;
	//ƽ̨������
	class NOA_API Platform {
	public:
		Platform();
		virtual ~Platform();
		virtual int Create(int width, int height,
				WindowMode windowMode,
				const std::string & gameName) = 0;
		std::shared_ptr<InputEvent> GetPlatformEvent();
		virtual bool CheckWindowClose() = 0;
		virtual void EventLoop() = 0;
		Vector<int> GetHardwareScreenSize();
		void Quit();
		uint32_t GetWindowID();

	protected:
		uint32_t windowID = 0;
		bool isRun = true;
		std::shared_ptr<InputEvent> ioEvent = nullptr;
		noa::Vector<int> hardwareScreenSize;

	};

	enum class NOA_API KeyCode; // ǰ������
	struct NOA_API MouseContext;
	struct NOA_API KeyboardContext;
	// ƽ̨�����¼���ĳ���
	class NOA_API InputEvent
	{
	private:
		friend class Input;
	
	public:
		InputEvent();
		virtual ~InputEvent();

		virtual bool GetKeyDown(noa::KeyCode key) = 0;
		virtual bool GetKeyDownOnce(KeyCode key) = 0;
		virtual bool GetKeyUp(noa::KeyCode key) = 0;

		virtual void* GetEventEntity() = 0;
		virtual void PollEvent(const std::function<void()> & quitCallback) = 0;
		void AddPollEventCallback(std::function<void()> callback);

		virtual void ResetKeyboardContext() = 0;
		virtual void UpdateKeyboardContext() = 0;

		virtual void ResetMouseContext() = 0;
		virtual void UpdateMouseContext() = 0;
		
		virtual void SetRelativeMouseMode(bool mode) = 0;
	protected:
		std::shared_ptr<MouseContext> mouseContext = nullptr;
		std::shared_ptr<KeyboardContext> keyboardContext = nullptr;

		noa::NoaEvent<void> eventPollAction;

	};

	extern std::shared_ptr<noa::Platform> platform;

}


#endif

