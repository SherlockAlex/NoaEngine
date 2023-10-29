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

	//T�Ķ�����NObject����
	#define NOBJECT(T) friend class noa::NObject<T>;

	//��ȫɾ��
	#define SAFE_DELETE(ptr) if(ptr) {delete ptr;(ptr) = nullptr;}
}



#endif // !NOAENGINE_NOBJECT