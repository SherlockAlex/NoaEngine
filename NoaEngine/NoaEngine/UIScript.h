#ifndef NOAENGINE_UISCRIPT_H
#define NOAENGINE_UISCRIPT_H

namespace noa {
	
	class UIBody;

	class UIScript 
	{
	private:
		void Delete(UIScript*& ptr);
	protected:
		UIScript(UIBody* body);
		virtual ~UIScript();
		virtual void Update() {};

	public:
		static UIScript* Create(UIBody* body);
		noa::UIBody* GetBody();
	private:
		friend class UIBody;

		UIBody* body = nullptr;
	};
}

#endif // !NOAENGINE_UISCRIPT_H

