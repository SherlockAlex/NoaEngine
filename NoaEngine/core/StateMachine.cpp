#include "StateMachine.h"
#include "NoaMath.h"
#include "NoaEngine.h"

namespace noa {
	StateMachine::StateMachine()
	{
		currentState = nullptr;
	}

	StateMachine::StateMachine(vector<State*> stateList)
	{
		this->stateList = stateList;
		if (!stateList.empty())
		{
			currentState = stateList[0];
		}
	}

	StateMachine::~StateMachine()
	{
		if (!stateList.empty())
		{
			for (int i = 0; i < stateList.size(); i++)
			{
				if (stateList[i] != nullptr)
				{
					delete stateList[i];
				}
			}
			stateList.clear();
		}
		
		Debug("Remove finite state machine");
	}

	StateMachine* StateMachine::Create()
	{
		return new StateMachine();
	}

	StateMachine* StateMachine::Create(vector<State*> stateList)
	{
		return new StateMachine(stateList);
	}

	void StateMachine::Delete()
	{
		delete this;
	}

	void StateMachine::PerformTransition(int transition)
	{
		//切换到下一个状态
		if (currentState == nullptr
			|| ContainKey<int, State*>(currentState->nextStates, transition)
			)
		{
			return;
		}
		currentState = currentState->nextStates[transition];
	}

	void StateMachine::AddState(State* state)
	{
		//添加状态
		stateList.push_back(state);
		if (currentState == nullptr)
		{
			currentState = state;
		}
	}

	void StateMachine::Act()
	{
		if (currentState == nullptr)
		{
			return;
		}
		currentState->OnUpdate();
	}

	void StateMachine::Reason()
	{
		if (currentState == nullptr)
		{
			return;
		}
		currentState->Reason();
	}

	State::State(StateMachine* stateMachine)
	{
		this->stateMachine = stateMachine;
	}

	State::~State()
	{
		Debug("Remove state successfully");
	}

	void State::AddTransition(int transition, State* nextState)
	{
		nextStates[transition] = nextState;
	}

	void State::SetTransition(int transition)
	{
		if (!ContainKey<int, State*>(nextStates, transition))
		{
			Debug("havn't state");
		}
		if (stateMachine->currentState != nullptr)
		{
			stateMachine->currentState->OnExit();
		}
		stateMachine->currentState = nextStates[transition];
		if (stateMachine->currentState != nullptr)
		{
			stateMachine->currentState->OnEnter();
		}

	}

}

