#ifndef NOAENGINE_NOBJECT_H
#define NOAENGINE_NOBJECT_H

namespace noa 
{
	template<class T>
	class NObject
	{
	protected:
		virtual ~NObject() {}
	public:
		static T* Create() 
		{
			return new T();
		};
		virtual void Delete() 
		{
			delete this;
		}
	};
}



#endif // !NOAENGINE_NOBJECT