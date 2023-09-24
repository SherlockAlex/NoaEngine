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
				// �����ڴ����ʧ�ܵ����
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
