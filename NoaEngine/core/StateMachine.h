#ifndef NOAENGINE_STATEMACHINE
#define NOAENGINE_STATEMACHINE

#include <vector>
#include <unordered_map>

using namespace std;

namespace noa 
{
	enum Transition {

	};

	/// <summary>
	/// ״̬�࣬��ÿһ��״̬�������һ����
	/// </summary>
	class State 
	{
	public:
		//��һ��״̬<transition,state>
		unordered_map<Transition, State*> nextStates;
	public:
		State();
		~State();
	public:
		virtual void Act() = 0;
		virtual void Reason() = 0;
	};

	/// <summary>
	/// ״̬���������������е�״̬
	/// </summary>
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


