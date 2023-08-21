#ifndef NOAENGINE_STATEMACHINE
#define NOAENGINE_STATEMACHINE

#include <vector>
#include <unordered_map>

using namespace std;

namespace noa 
{
	enum Transition {

	};

	class State 
	{
	public:
		//ÏÂÒ»¸ö×´Ì¬<transition,state>
		unordered_map<Transition, State*> nextStates;
	public:
		State();
		~State();
	public:
		virtual void Act() = 0;
		virtual void Reason() = 0;
	};

	class StateMachine
	{
	public:
		State* currentState;
		vector<State*> stateList;
	public:
		void PerformTransition(Transition transition);
		void Act();
		void Reason();
	};
}

#endif // !NOAENGINE_STATEMACHINE


