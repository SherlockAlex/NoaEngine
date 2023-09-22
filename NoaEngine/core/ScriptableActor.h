#ifndef NOAENGINE_SCRIPTABLEACTOR_H
#define NOAENGINE_SCRIPTABLEACTOR_H

namespace noa 
{
	class ScriptableActor {
	public:
		virtual ~ScriptableActor() {}

		virtual void Delete() { delete this; }

	};
}

#endif // !NOAENGINE_SCRIPTABLEACTOR_H

