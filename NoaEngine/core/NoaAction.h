#ifndef NOAENGINE_NOAACTION_H
#define NOAENGINE_NOAACTION_H

#include <functional>
#include <vector>

/// <summary>
/// ������ί�У�������ָ�룬�䱾������������ݺ���
/// </summary>

namespace noa {
	template <class T>
	class NoaEvent
	{
	private:
		std::vector<std::function<T>> functions;
	public:
		
		//ִ���¼�
		void Invoke() {
			const int functionCount = functions.size();
			for (int i = 0; i < functionCount; i++)
			{
				functions[i]();
			}
		}

		//�����¼�
		void operator += (std::function<T> func)
		{
			functions.push_back(func);
		}

		//ע���¼�
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


