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
	class NoaEvent
	{
	private:
		std::vector<std::function<void()>> functions;
	public:
		
		//执行事件
		void Invoke();

		//订阅事件
		void operator += (std::function<void()> func);

		//注销事件
		void operator -= (std::function<void()> func)
		{
			/*auto it = std::find(functions.begin(), functions.end(), func);
			if (it != functions.end())
			{
				functions.erase(it);
			}*/
		}

	};
	
}


#endif // !NOAENGINE_NOAACTION_H


