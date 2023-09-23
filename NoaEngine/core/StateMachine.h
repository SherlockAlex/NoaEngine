#ifndef NOAENGINE_STATEMACHINE
#define NOAENGINE_STATEMACHINE

#include <vector>
#include <unordered_map>
#include <memory>
#include "ScriptableActor.h"
#include "ActorComponent.h"

/*
* 此状态机可以对他进行一个优化，注意到状态机的Reason()和Act(),一群小怪可能持有的状态机是相同的，不同的地方是自身属性输入,
* 状态机可以提供计算的方法流程，但是输入是输入小怪
*/

namespace noa 
{

	
	class State;
	class Actor;
	class StateMachine;

	/// <summary>
	/// 状态类，将每一个状态都必须继承这个类，并且只能分配到堆空间
	/// 请覆写方法 T Create() 和 void Delete()
	/// </summary>
	class State 
	{
	protected:
		StateMachine* stateMachine;
	public:
		std::unordered_map<int, State*> nextStates;
	protected:
		State(StateMachine* stateMachine);
		virtual ~State() {};
	public:
		virtual void Delete() = 0;

		void AddTransition(int transition,State* nextState);
		void SetTransition(int transition);

	public:

		virtual void OnEnter() {};
		virtual void OnUpdate() {};
		virtual void Reason() {};
		virtual void OnExit() {};
	};

	/// <summary>
	/// 状态机，用来管理所有的状态,注意所有的状态类都必须继承自State抽象类
	/// </summary>
	class StateMachine:ActorComponent
	{
	protected:
		std::vector<State*> stateList;
	public:
		State* currentState = nullptr;
	protected:
		StateMachine(Actor * actor);
		virtual ~StateMachine() override;

	public:
		static StateMachine* Create(Actor* actor);
		virtual void Delete() override;

		virtual void PerformTransition(int transition);
		virtual void AddState(State* state);
		virtual void Act();
		virtual void Reason();

		virtual void Update() override;

	};
}

#endif // !NOAENGINE_STATEMACHINE


