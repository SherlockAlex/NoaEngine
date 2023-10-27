#ifndef NOAENGINE_NOAACTION_H
#define NOAENGINE_NOAACTION_H

#include <functional>
#include <vector>
#include <string>

/// <summary>
/// 使用事件之前，希望你能区分函数指针，委托，以及事件
/// 函数指针:顾名思义，就是一个指针，一个容器，用来存放函数在内存中的地址，方便计算机通过这个地址，去调用函数中的内容
/// 委托:请你和函数指针加以区别，委托请你理解为一个动作，这个动作需要用户自定义，委托至少要有一个动作，而在程序中通常动作表示一些一系列方法
///		所以委托所拥有的并不是一个函数的地址，而是通过函数的地址，去实现这个委托相应的动作，请注意委托是一个对象，他传递的不单单是函数，而是这个对象本身
/// 
/// 事件:事件基于委托实现，事件是在发生某件事的时候，让事件的接收方自己去执行对应的动作，及委托中的内容
/// 
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


