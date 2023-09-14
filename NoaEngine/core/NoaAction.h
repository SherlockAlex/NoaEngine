#ifndef NOAENGINE_NOAACTION_H
#define NOAENGINE_NOAACTION_H

#include <functional>
#include <vector>

/// <summary>
/// 类似与委托，即函数指针，其本身可以用来传递函数
/// </summary>

namespace noa {
	template <class T>
	class NoaEvent
	{
	private:
		std::vector<std::function<T>> functions;
	public:
		
		//执行事件
		void Invoke() {
			const int functionCount = functions.size();
			for (int i = 0; i < functionCount; i++)
			{
				functions[i]();
			}
		}

		//订阅事件
		void operator += (std::function<T> func)
		{
			functions.push_back(func);
		}

		//注销事件
		void operator -= (std::function<T> func)
		{
			auto it = std::find(functions.begin(), functions.end(), func);
			if (it != functions.end())
			{
				functions.erase(it);
			}
		}
	};
	
}


#endif // !NOAENGINE_NOAACTION_H


