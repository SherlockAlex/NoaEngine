#ifndef NOAENGINE_UISCRIPT_H
#define NOAENGINE_UISCRIPT_H

#include "NObject.h"

namespace noa {
	class UIBody;

	class UIScript:noa::NoaObject
	{
	private:
		void Delete(UIScript*& ptr);
	protected:
		UIScript(UIBody* body);
		virtual ~UIScript();
		virtual void Update() {};

		virtual void OnEnter() {};
		virtual void OnStay() {};
		virtual void OnExit() {};

	public:
		noa::UIBody* GetBody();
	private:
		friend class UIBody;
		friend class noa::NObject<UIScript>;

		UIBody* body = nullptr;
	};
}

#define UISCRIPT(T) template<class T> friend class noa::NObject;

#endif // !NOAENGINE_UISCRIPT_H

