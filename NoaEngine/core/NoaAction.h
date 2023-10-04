#ifndef NOAENGINE_NOAACTION_H
#define NOAENGINE_NOAACTION_H

#include <functional>
#include <vector>
#include <string>

/// <summary>
/// 类似与委托，即函数指针，其本身可以用来传递函数
/// </summary>

namespace noa 
{
	template<typename ...Args>
	class NoaEvent 
	{
	private:
		std::vector<std::function<void(Args...)>> functions;
	public:

		void Invoke(Args... args)
		{
			const int functionCount = static_cast<int>(functions.size());
			for (int i = 0; i < functionCount; i++)
			{
				functions[i](args);
			}
		}

		//订阅事件
		void operator += (std::function<void(Args...)> func) {
			functions.push_back(func);
		}

	};

	template<>
	class NoaEvent<void>
	{
	private:
		std::vector<std::function<void()>> functions;
	public:

		void Invoke()
		{
			const int functionCount = static_cast<int>(functions.size());
			for (int i = 0; i < functionCount; i++)
			{
				functions[i]();
			}
		}

		void operator += (std::function<void()> func) {
			functions.push_back(func);
		}
	};

}


#endif // !NOAENGINE_NOAACTION_H


