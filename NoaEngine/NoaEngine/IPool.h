#ifndef NOAENGINE_IPOOL_H
#define NOAENGINE_IPOOL_H

namespace noa {
	
	/***********************
	* IPool,PoolObject,IPoolObjectFactory
	* 
	* IPool:对象池接口
	* 
	* 声明对象池的作用
	* 
	***********************/

	template<class T>
	class IPool {
	protected:
		virtual ~IPool() {};
	
	public:

		// 预热对象池
		virtual void Prewarm(int count) = 0;

		// 申请对象
		virtual T* Request() = 0;

		// 返回对象池
		virtual void Return(T* member) = 0;

	};

}

#endif //NOAENGINE_IPOOL_H
