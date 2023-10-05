#ifndef NOAENGINE_NOBJECT_H
#define NOAENGINE_NOBJECT_H

/**************************
* 
* NObject 模块
* 作用:用于将Actor和ActorComponent分配到堆上
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

	//T的对象由NObject管理
	#define NOBJECT(T) friend class NObject<T>;

}



#endif // !NOAENGINE_NOBJECT