#ifndef NOAENGINE_NOBJECT_H
#define NOAENGINE_NOBJECT_H

namespace noa 
{
	class NObject
	{
	protected:
		virtual ~NObject() {}
	public:

		template<typename T,typename ...Args>
		static T* Create(Args... args) 
		{
			return new T(args);
		};

		template<typename T>
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