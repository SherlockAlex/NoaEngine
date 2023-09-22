#ifndef NOAENGINE_STATEMACHINE
#define NOAENGINE_STATEMACHINE

#include <vector>
#include <unordered_map>
#include <memory>

/*
* ��״̬�����Զ�������һ���Ż���ע�⵽״̬����Reason()��Act(),һȺС�ֿ��ܳ��е�״̬������ͬ�ģ���ͬ�ĵط���������������,
* ״̬�������ṩ����ķ������̣���������������С��
*/

namespace noa 
{

	/// <summary>
	/// ״̬�࣬��ÿһ��״̬�������һ����
	/// </summary>
	class State;
	class Actor;
	class StateMachine;
	
	class State 
	{
	protected:
		StateMachine* stateMachine;
	public:
		std::unordered_map<int, State*> nextStates;
	public:
		State(StateMachine* stateMachine);
		virtual ~State();
	public:
		static State* Create(StateMachine* stateMachine);
		virtual void Delete();

		void AddTransition(int transition,State* nextState);
		void SetTransition(int transition);

	public:
		virtual void OnEnter() {};
		virtual void OnUpdate(Actor * owner) {};
		virtual void Reason(Actor* owner) {};
		virtual void OnExit() {};
	};

	/// <summary>
	/// ״̬���������������е�״̬
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
		virtual void Act(Actor* owner);
		virtual void Reason(Actor* owner);
	};
}

#endif // !NOAENGINE_STATEMACHINE


