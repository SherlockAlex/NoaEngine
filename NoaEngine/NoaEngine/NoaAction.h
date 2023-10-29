#ifndef NOAENGINE_NOAACTION_H
#define NOAENGINE_NOAACTION_H

#include <functional>
#include <vector>
#include <string>

/// <summary>
/// ʹ���¼�֮ǰ��ϣ���������ֺ���ָ��,�ص�������ί�У��Լ��¼�
/// ����ָ��:����˼�壬����һ��ָ�룬һ��������������ź������ڴ��еĵ�ַ����������ͨ�������ַ��ȥ���ú����е�����
/// 
/// �ص�����:���ǽ�������Ϊ�������ݸ���һ��������Ȼ������һ��������������е��ã�ע��ͺ���ָ�������
///			�ص�����ǿ�����Ǻ�����������ָ��ǿ������ָ�룬һ���Ǻ�����һ����ָ�룬ֻ�ǻص�������ʵ�ַ�ʽ��ͨ������ָ��ʵ�ֵĶ��ѣ����Ǹ����ǲ�ͬ��
/// 
/// ί��:����ͺ���ָ���������ί���������Ϊһ�����������������Ҫ�û��Զ��壬ί������Ҫ��һ�����������ڳ�����ͨ��������ʾһЩһϵ�з���
///		����ί�а󶨵Ĳ����Ǻ����ĵ�ַ�����Ǻ�������ȥʵ�����ί����Ӧ�Ķ��������ͻص���������������ί�д��ݵ���һ�����������ݵĲ������Ǻ������������������ֻ������������ϰ���һϵ�к�����ͨ���������ȥ������Щ����(����)
/// 
/// �¼�:�¼���һ�������ί�У���ί�н���һ����װ������ȫ���������ڶ���֮���ͨ��
///		������ĳ���µ�ʱ���¼�Դ(������¼�����)��ȥִ��ί�����Լ�ע��Ķ���
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


