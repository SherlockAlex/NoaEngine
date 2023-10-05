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

	class NOAObject {
	protected:
		virtual ~NOAObject() {}
	};

	extern void InitNObject(NOAObject* obj);

	template<typename T>
	class NObject final
	{
	public:

		template<typename ...Args>
		static T* Create(Args... args) {
			T* obj = new T(args...);
			InitNObject(obj);
			return obj;
		}

		static T* Create() {
			T* obj = new T();
			InitNObject(obj);
			return obj;
		}

	};

	//T的对象由NObject管理
	#define NOBJECT(T) friend class noa::NObject<T>;

	//安全删除
	#define SAFE_DELETE(ptr) if(ptr) {delete ptr;(ptr) = nullptr;}
}



#endif // !NOAENGINE_NOBJECT