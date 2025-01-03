#ifndef NOAENGINE_NOBJECT_H
#define NOAENGINE_NOBJECT_H

/**************************
* 
* NObject 模块
* 用于将Actor和ActorComponent分配到堆上
* 同时规定一套定式，规范Actor和ActorComponent的构造参数
* 
**************************/

#include "Debug.h"

#include "Core.h"

namespace noa 
{

	class NOA_API NoaObject
	{
	protected:
		virtual ~NoaObject() {}
	};

	extern NOA_API void InitNObject(NoaObject* obj);

	class NOA_API Actor;
	class NOA_API Scene;
	class NOA_API UIBody;
	class NOA_API UIContainer;

	template<typename T>
	class NOA_API NObject final
	{
	public:

		static T* Create() {
			T* obj = new T();
			return obj;
		}

		static T* Create(Scene * scene) 
		{
			if (scene == nullptr) 
			{
				noa::Debug::Error("create actor failed,this scene is nullptr");
				return nullptr;
			}
			T* obj = new T(scene);
			InitNObject(obj);
			return obj;
		}

		static T* Create(Actor * actor) 
		{
			if (actor == nullptr)
			{
				noa::Debug::Error("create component failed,this actor is nullptr");
				return nullptr;
			}
			T* obj = new T(actor);
			InitNObject(obj);
			return obj;
		}

		//创建UI控件
		static T* Create(UIContainer* container) {
			if (container == nullptr)
			{
				return nullptr;
			}
			T* obj = new T(container);
			return obj;
		}

		//创建UI控件脚本
		static T* Create(UIBody* body) 
		{
			if (body == nullptr)
			{
				noa::Debug::Error("create UIScript object failed,this UIBody is nullptr");
				return nullptr;
			}
			T* obj = new T(body);
			return obj;
		}

	};

}

//T的对象由NObject管理
#define NOBJECT(T) friend class noa::NObject<T>;

//安全删除
#define SAFE_DELETE(ptr) if(ptr) {delete ptr;(ptr) = nullptr;}



#endif // !NOAENGINE_NOBJECT