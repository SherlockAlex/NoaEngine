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
	/// 状态类，将每一个状态都抽象乘一个类
	/// </summary>
	class State 
	{
	public:
		//下一个状态<transition,state>
		unordered_map<Transition, State*> nextStates;
	public:
		State();
		~State();
	public:
		virtual void Act() = 0;
		virtual void Reason() = 0;
	};

	/// <summary>
	/// 状态机，用来管理所有的状态
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


