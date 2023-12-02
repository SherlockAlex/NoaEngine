#ifndef NOAENGINE_IOSTRUCT_H
#define NOAENGINE_IOSTRUCT_H

#include "NoaMath.h"
#include "IOCode.h"
#include "Core.h"

#include <unordered_map>

namespace noa {
	typedef struct NOA_API KeyState
	{
		bool performed = false;
		bool wasPerformedOnce = false;
		bool canceled = false;
		bool wasReleased = true;
	}KeyState;

	typedef struct NOA_API KeyboardContext {
		std::unordered_map<noa::KeyCode, noa::KeyState> keyMap;
		KeyboardContext();
	}KeyboardContext;

	//鼠标输入事件上下文
	typedef struct NOA_API MouseContext
	{
		Vector<double> position;
		Vector<double> delta;
		Vector<double> wheel;
		bool motion = false;

		std::unordered_map<noa::MouseButton, noa::KeyState> mouseKey;
		MouseContext();

	}MouseContext;
}

#endif // !NOAENGINE_IOSTRUCT_H

