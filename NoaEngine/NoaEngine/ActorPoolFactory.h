#ifndef NOAENGINE_ACTORPOOLFACTORY_H
#define NOAENGINE_ACTORPOOLFACTORY_H

#include "Actor.h"
#include "IPoolObjectFactroy.h"

namespace noa {
	template<class T>
	class ActorFactory:public IPoolObjectFactroy<T> 
	{
		// 具体的构造函数请你更具你的对象构造函数进行封装
	public:
		virtual T* Create() = 0;
	};
}



#endif // !NOAENGINE_ACTORPOOLFACTORY_H


