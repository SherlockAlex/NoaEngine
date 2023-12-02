#ifndef NOAENGINE_IPOOLOBJECTFACTORY_H
#define NOAENGINE_IPOOLOBJECTFACTORY_H

#include "Core.h"

namespace noa
{
	template<class T>
	class NOA_API IPoolObjectFactroy
	{
	public:
		virtual T* Create() = 0;
	};
}

#endif // !NOAENGINE_IPOOLOBJECTFACTORY_H



