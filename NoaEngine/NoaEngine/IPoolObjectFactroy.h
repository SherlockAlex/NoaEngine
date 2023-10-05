#ifndef NOAENGINE_IPOOLOBJECTFACTORY_H
#define NOAENGINE_IPOOLOBJECTFACTORY_H

namespace noa
{
	template<class T>
	class IPoolObjectFactroy
	{
	public:
		virtual T* Create() = 0;
	};
}

#endif // !NOAENGINE_IPOOLOBJECTFACTORY_H



