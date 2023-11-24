#ifndef NOAENGINE_NOBJECT_H
#define NOAENGINE_NOBJECT_H

/**************************
* 
* NObject ģ��
* ���ڽ�Actor��ActorComponent���䵽����
* ͬʱ�涨һ�׶�ʽ���淶Actor��ActorComponent�Ĺ������
* 
**************************/

#include "Debug.h"

namespace noa 
{

	class NoaObject 
	{
	protected:
		virtual ~NoaObject() {}
	};

	extern void InitNObject(NoaObject* obj);

	class Actor;
	class Scene;
	class UIBody;

	template<typename T>
	class NObject final
	{
	public:

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

	//T�Ķ�����NObject����
	#define NOBJECT(T) friend class noa::NObject<T>;

	//��ȫɾ��
	#define SAFE_DELETE(ptr) if(ptr) {delete ptr;(ptr) = nullptr;}
}



#endif // !NOAENGINE_NOBJECT