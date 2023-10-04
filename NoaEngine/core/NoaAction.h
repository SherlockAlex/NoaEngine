#ifndef NOAENGINE_NOAACTION_H
#define NOAENGINE_NOAACTION_H

#include <functional>
#include <vector>
#include <string>

/// <summary>
/// ������ί�У�������ָ�룬�䱾������������ݺ���
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

		//�����¼�
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


