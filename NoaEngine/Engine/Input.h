#ifndef NOAGAMEENGINE_INPUTSYSTEM_H
#define NOAGAMEENGINE_INPUTSYSTEM_H

#include "SDLHelper.h"
#include "NoaMath.h"
#include "Graphic.h"
#include "NoaAction.h"
#include "IOCode.h"
#include "IOStruct.h"

#include "Core.h"

#include <unordered_map>
#include <memory>


namespace noa
{

	class NOA_API InputEvent;
	class NOA_API Input final
	{
	private:
		static std::shared_ptr<InputEvent> ioEvent;
	public:

		static void InitInputSystem(std::shared_ptr<InputEvent> e);

		static bool GetKeyDown(KeyCode key);
		static bool GetKeyDownOnce(KeyCode key);
		static bool GetKeyUp(KeyCode key);

		static bool GetMouseMoveState();
		static bool GetMouseKeyDown(MouseButton mouseButton);
		static bool GetMouseKeyDownOnce(MouseButton mouseButton);
		static bool GetMouseKeyUp(MouseButton mouseButton);

		static void SetRelativeMouseMode(bool mode);
		static Vector<double>& GetMouseMoveDelta();
		static Vector<double>& GetMousePosition();
		static Vector<double>& GetMouseWheel();

	};

	//目标是这样的，一个按钮绑定一个事件，这个事件的返回值可以是Vector<double>,也可以是bool,最后使用ReadValue<>()开发者便可以读取到数据
}


#endif // !NOAGAMEENGINE_INPUTSYSTEM_H