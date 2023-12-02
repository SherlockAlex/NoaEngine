#ifndef NOAENGINE_ACTORPOOLFACTORY_H
#define NOAENGINE_ACTORPOOLFACTORY_H

#include "Core.h"
#include "Actor.h"
#include "IPoolObjectFactroy.h"

/*
* ActorFactory
* 作用:生成制作Actor及其派生类的对象池，主要用于射击游戏
*/

namespace noa {
	template<class T>
	class NOA_API ActorFactory:public IPoolObjectFactroy<T>
	{
	public:
		virtual T* Create() {
			if (activeScene == nullptr)
			{
				return noa::NObject<T>::Create(noa::SceneManager::GetActiveScene());
			}
			return noa::NObject<T>::Create(this->activeScene);
		};
		
		void SetActiveScene(noa::Scene* scene) {
			this->activeScene = scene;
		}
	protected:
		noa::Scene* activeScene = nullptr;
	};
}



#endif // !NOAENGINE_ACTORPOOLFACTORY_H


