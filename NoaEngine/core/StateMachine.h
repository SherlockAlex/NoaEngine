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
	private:
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
		virtual void Act() = 0;
		virtual void Reason() = 0;
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
	public:
		StateMachine();
		StateMachine(vector<State*> stateList);
		void PerformTransition(int transition);
		void AddState(State* state);
		void Act();
		void Reason();
	};
}

#endif // !NOAENGINE_STATEMACHINE


