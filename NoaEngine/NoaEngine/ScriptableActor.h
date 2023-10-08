#ifndef NOAENGINE_SCRIPTABLEACTOR_H
#define NOAENGINE_SCRIPTABLEACTOR_H

namespace noa 
{
	class ScriptableActor {
	private:
		ScriptableActor() {}
		virtual ~ScriptableActor() {}
	public:
		void Delete() { delete this; }

	};
}

#endif // !NOAENGINE_SCRIPTABLEACTOR_H

