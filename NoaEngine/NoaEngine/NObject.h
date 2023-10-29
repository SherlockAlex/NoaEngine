#ifndef NOAENGINE_NOBJECT_H
#define NOAENGINE_NOBJECT_H

/**************************
* 
* NObject 模块
* 用于将Actor和ActorComponent分配到堆上
* 同时规定一套定式，规范Actor和ActorComponent的构造参数
* 
**************************/

namespace noa 
{

	class NOAObject {
	protected:
		virtual ~NOAObject() {}
	};

	extern void InitNObject(NOAObject* obj);

	class Actor;
	class Scene;

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

		static T* Create(Scene * scene) 
		{
			T* obj = new T(scene);
			InitNObject(obj);
			return obj;
		}

		static T* Create(Actor * actor) 
		{
			T* obj = new T(actor);
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