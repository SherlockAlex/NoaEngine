#ifndef NOAENGINE_IPOOL_H
#define NOAENGINE_IPOOL_H

namespace noa {
	
	/***********************
	* IPool,PoolObject,IPoolObjectFactory
	* 
	* IPool:����ؽӿ�
	* 
	* ��������ص�����
	* 
	***********************/

	template<class T>
	class IPool {
	protected:
		virtual ~IPool() {};
	
	public:

		// Ԥ�ȶ����
		virtual void Prewarm(int count) = 0;

		// �������
		virtual T* Request() = 0;

		// ���ض����
		virtual void Return(T* member) = 0;

	};

}

#endif //NOAENGINE_IPOOL_H
