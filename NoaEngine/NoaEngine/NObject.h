#ifndef NOAENGINE_NOBJECT_H
#define NOAENGINE_NOBJECT_H

/**************************
* 
* NObject ģ��
* ����:���ڽ�Actor��ActorComponent���䵽����
* 
**************************/

namespace noa 
{
	template<typename T>
	class NObject final
	{
	public:

		template<typename ...Args>
		static T* Create(Args... args) 
		{
			return new T(args...);
		};

		static T* Create()
		{
			return new T();
		};

	};

	//T�Ķ�����NObject����
	#define NOBJECT(T) friend class NObject<T>;

}



#endif // !NOAENGINE_NOBJECT