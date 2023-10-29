#ifndef NOAENGINE_NOAACTION_H
#define NOAENGINE_NOAACTION_H

#include <functional>
#include <vector>
#include <string>

/// <summary>
/// 使用事件之前，希望你能区分函数指针,回调函数，委托，以及事件
/// 函数指针:顾名思义，就是一个指针，一个容器，用来存放函数在内存中的地址，方便计算机通过这个地址，去调用函数中的内容
/// 
/// 回调函数:就是将函数做为参数传递给另一个函数，然后由另一个函数看情况进行调用，注意和函数指针的区别
///			回调函数强调的是函数，而函数指针强调的是指针，一个是函数，一个是指针，只是回调函数的实现方式是通过函数指针实现的而已，但是概念是不同的
/// 
/// 委托:请你和函数指针加以区别，委托请你理解为一个动作，这个动作需要用户自定义，委托至少要有一个动作，而在程序中通常动作表示一些一系列方法
///		所以委托绑定的并不是函数的地址，而是函数本身，去实现这个委托相应的动作，他和回调函数的区别在于委托传递的是一个对象，他传递的不单单是函数，而是这个对象本身，只是这个对象身上绑定着一系列函数，通过这个对象去调用这些函数(动作)
/// 
/// 事件:事件是一个特殊的委托，对委托进行一个封装，更安全，常常用于对象之间的通信
///		当发生某件事的时候，事件源(定义的事件对象)会去执行委托中以及注册的动作
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
				functions[i](args...);
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


