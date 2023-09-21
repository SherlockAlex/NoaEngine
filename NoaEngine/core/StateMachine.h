#ifndef NOAENGINE_STATEMACHINE
#define NOAENGINE_STATEMACHINE

#include <vector>
#include <unordered_map>

using namespace std;

namespace noa 
{

	/// <summary>
	/// 状态类，将每一个状态都抽象乘一个类
	/// </summary>
	class State;
	class StateMachine;
	
	class State 
	{
	protected:
		StateMachine* stateMachine;
	public:
		//下一个状态<transition,state>
		unordered_map<int, State*> nextStates;
	public:
		State(StateMachine * stateMachine);
		~State();

		void AddTransition(int transition,State* nextState);
		void SetTransition(int transition);

	public:
		virtual void OnEnter() {};
		virtual void OnUpdate() {};
		virtual void Reason() {};
		virtual void OnExit() {};
	};

	/// <summary>
	/// 状态机，用来管理所有的状态
	/// </summary>
	class StateMachine
	{
	private:
		vector<State*> stateList;
	public:
		State* currentState;
	protected:
		StateMachine();
		StateMachine(vector<State*> stateList);
		~StateMachine();

	public:
		static StateMachine* Create();
		static StateMachine* Create(vector<State*> stateList);
		virtual void Delete();

		virtual void PerformTransition(int transition);
		virtual void AddState(State* state);
		virtual void Act();
		virtual void Reason();
	};
}

#endif // !NOAENGINE_STATEMACHINE


