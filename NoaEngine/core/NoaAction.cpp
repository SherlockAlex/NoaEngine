#include "NoaAction.h"
#include "NoaEngine.h"

namespace noa {
	

	
	void NoaEvent::Invoke()
	{
		const int functionCount = functions.size();
		for (int i = 0; i < functionCount; i++)
		{
			try {
				functions[i]();
			}
			catch (const std::bad_alloc& e) {
				// 处理内存分配失败的情况
				Debug("Memory allocation failed: " + std::string(e.what()));
				continue;
			}
		}
	}

	void NoaEvent::operator+=(std::function<void()> func)
	{
		functions.push_back(func);
	}

}
