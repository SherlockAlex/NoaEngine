#ifndef NOAENGINE_POOLOBJECT_H
#define NOAENGINE_POOLOBJECT_H

#include <queue>

#include "IPool.h"
#include "IPoolObjectFactroy.h"
#include "Debug.h"

#include "Core.h"

namespace noa {
	template<class T>
	class NOA_API ObjectPool :public IPool<T> {
	private:
		bool isPrewarmed = false;
	protected:
		std::queue<T*> available;
		IPoolObjectFactroy<T> * factory = nullptr;

		virtual ~ObjectPool()
		{

			while (!available.empty()) 
			{
				available.pop();
			}

			isPrewarmed = false;

		}

	public:
		void SetFactory(IPoolObjectFactroy<T> * factory) 
		{
			this->factory = factory;
		}

	protected:
		T* Create() {
			if (factory == nullptr)
			{
				Debug::Error("Pool Factory is nullptr");
				return nullptr;
			}

			return factory->Create();

		}
		
		virtual void Prewarm(int number) override {
			if (isPrewarmed) 
			{
				Debug::Error("ObjectPool is prewarmed");
				return;
			}

			for (int i = 0;i<number;i++)
			{
				available.push(Create());
			}

			isPrewarmed = true;

		}

		virtual T* Request() override {
			if (available.size()>0)
			{
				T* result = available.front();
				available.pop();
				return result;
			}
			return Create();
		}

		virtual void Return(T* member) override 
		{
			available.push(member);
		}

	};
}

#endif
