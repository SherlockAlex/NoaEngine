#ifndef NOAENGINE_NOBJECT_H
#define NOAENGINE_NOBJECT_H

/**************************
* 
* NObject ģ��
* ���ڽ�Actor��ActorComponent���䵽����
* ͬʱ�涨һ�׶�ʽ���淶Actor��ActorComponent�Ĺ������
* 
**************************/

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

	template<typename T>
	class NObject final
	{
	public:

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

		template<typename ...Args>
		static T* Create(Args... arg)
		{
			T* obj = new T(arg...);
			InitNObject(obj);
			return obj;
		}

	};

	//T�Ķ�����NObject����
	#define NOBJECT(T) friend class noa::NObject<T>;

	//��ȫɾ��
	#define SAFE_DELETE(ptr) if(ptr) {delete ptr;(ptr) = nullptr;}
}



#endif // !NOAENGINE_NOBJECT