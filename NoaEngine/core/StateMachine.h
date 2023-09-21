#ifndef NOAENGINE_STATEMACHINE
#define NOAENGINE_STATEMACHINE

#include <vector>
#include <unordered_map>

using namespace std;

namespace noa 
{

	/// <summary>
	/// ״̬�࣬��ÿһ��״̬�������һ����
	/// </summary>
	class State;
	class StateMachine;
	
	class State 
	{
	protected:
		StateMachine* stateMachine;
	public:
		//��һ��״̬<transition,state>
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
	/// ״̬���������������е�״̬
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


