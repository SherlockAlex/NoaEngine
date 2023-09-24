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
	class NoaEvent
	{
	private:
		std::vector<std::function<void()>> functions;
	public:
		
		//ִ���¼�
		void Invoke();

		//�����¼�
		void operator += (std::function<void()> func);

		//ע���¼�
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


