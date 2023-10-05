#ifndef NOAENGINE_ACTORPOOLFACTORY_H
#define NOAENGINE_ACTORPOOLFACTORY_H

#include "Actor.h"
#include "IPoolObjectFactroy.h"

namespace noa {
	template<class T>
	class ActorFactory:public IPoolObjectFactroy<T> 
	{
		// ����Ĺ��캯�����������Ķ����캯�����з�װ
	public:
		virtual T* Create() = 0;
	};
}



#endif // !NOAENGINE_ACTORPOOLFACTORY_H


