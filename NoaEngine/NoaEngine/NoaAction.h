#ifndef NOAENGINE_NOAACTION_H
#define NOAENGINE_NOAACTION_H

#include <functional>
#include <vector>
#include <string>

/// <summary>
/// ʹ���¼�֮ǰ��ϣ���������ֺ���ָ�룬ί�У��Լ��¼�
/// ����ָ��:����˼�壬����һ��ָ�룬һ��������������ź������ڴ��еĵ�ַ����������ͨ�������ַ��ȥ���ú����е�����
/// ί��:����ͺ���ָ���������ί���������Ϊһ�����������������Ҫ�û��Զ��壬ί������Ҫ��һ�����������ڳ�����ͨ��������ʾһЩһϵ�з���
///		����ί����ӵ�еĲ�����һ�������ĵ�ַ������ͨ�������ĵ�ַ��ȥʵ�����ί����Ӧ�Ķ�������ע��ί����һ�����������ݵĲ������Ǻ������������������
/// 
/// �¼�:�¼�����ί��ʵ�֣��¼����ڷ���ĳ���µ�ʱ�����¼��Ľ��շ��Լ�ȥִ�ж�Ӧ�Ķ�������ί���е�����
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


