#ifndef NOAENGINE_STATEMACHINE
#define NOAENGINE_STATEMACHINE

#include <vector>
#include <unordered_map>
#include <memory>

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
		std::unordered_map<int, State*> nextStates;
	protected:
		State(StateMachine* stateMachine);
		virtual ~State();
	public:
		static State* Create(StateMachine* stateMachine);
		virtual void Delete();

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
		std::vector<State*> stateList;
	public:
		State* currentState = nullptr;
	public:
		StateMachine();
		virtual ~StateMachine();

	public:
		static StateMachine* Create();
		virtual void Delete();

		virtual void PerformTransition(int transition);
		virtual void AddState(State* state);
		virtual void Act();
		virtual void Reason();
	};
}

#endif // !NOAENGINE_STATEMACHINE


