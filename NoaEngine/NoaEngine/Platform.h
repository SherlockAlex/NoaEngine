#ifndef NOAENGINE_PLATFORM
#define NOAENGINE_PLATFORM


#include <string>
#include <functional>
#include <chrono>
#include <memory>


namespace noa {

	//窗口类型
	enum class WindowMode
	{
		FULLSCREEN = 1,
		WINDOW = 2048
	};

	class InputEvent;
	//平台抽象类
	class Platform {
	protected:
		//系统时钟

		uint32_t windowID = 0;

		bool isRun = true;

		std::shared_ptr<InputEvent> e = nullptr;

	public:

		Platform();
		virtual ~Platform();

		//创建窗口
		virtual int Create(int width, int height,
				WindowMode windowMode,
				const std::string & gameName) = 0;

		std::shared_ptr<InputEvent> GetPlatformEvent();

		//确定窗口关闭
		virtual bool CheckWindowClose() = 0;

		virtual void EventLoop() = 0;

		void Quit();

		uint32_t GetWindowID();


	};

	enum class KeyCode; // 前置声明
	struct MouseContext;
	// 平台输入事件类的抽象
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
		virtual void UpdateMouseContext() = 0;
		virtual void SetRelativeMouseMode(bool mode) = 0;
	};

}


#endif

