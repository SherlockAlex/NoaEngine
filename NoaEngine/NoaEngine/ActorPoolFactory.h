#ifndef NOAENGINE_ACTORPOOLFACTORY_H
#define NOAENGINE_ACTORPOOLFACTORY_H

#include "Actor.h"
#include "IPoolObjectFactroy.h"

/*
* ActorFactory
* ����:��������Actor����������Ķ���أ���Ҫ���������Ϸ
*/

namespace noa {
	template<class T>
	class ActorFactory:public IPoolObjectFactroy<T> 
	{
	public:
		virtual T* Create() = 0;
		
		void SetActiveScene(noa::Scene* scene) {
			this->activeScene = scene;
		}
	protected:
		noa::Scene* activeScene = nullptr;
	};
}



#endif // !NOAENGINE_ACTORPOOLFACTORY_H


