#include "StateMachine.h"
#include "NoaMath.h"
#include "NoaEngine.h"

namespace noa {

	StateMachine::StateMachine(Actor * actor):ActorComponent(actor)
	{
		currentState = nullptr;
	}

	StateMachine::~StateMachine()
	{
		currentState = nullptr;
		if (!stateList.empty())
		{

			auto last = std::unique(stateList.begin(), stateList.end());
			stateList.erase(last, stateList.end());

			for (int i = 0;i<stateList.size();i++)
			{
				if (stateList[i] == nullptr)
				{
					continue;
				}
				stateList[i]->Delete();
			}
			stateList.clear();
		}
		
		stateList.clear();
		Debug("Remove finite state machine");
	}

	StateMachine* StateMachine::Create(Actor* actor)
	{
		return new StateMachine(actor);
	}

	/*StateMachine* StateMachine::Create(vector<shared_ptr<State>> stateList)
	{
		return new StateMachine(stateList);
	}*/

	void StateMachine::Delete()
	{
		delete this;
	}

	void StateMachine::PerformTransition(int transition)
	{
		//切换到下一个状态
		if (currentState == nullptr
			|| !ContainKey<int, State*>(currentState->nextStates, transition)
			|| currentState->nextStates[transition] == nullptr
			)
		{
			return;
		}

		currentState->OnExit();
		currentState = currentState->nextStates[transition];
		currentState->OnEnter();

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

	void StateMachine::Update()
	{
		if (currentState == nullptr)
		{
			return;
		}
		currentState->Reason();
		currentState->OnUpdate();
	}

	State::State(StateMachine* stateMachine)
	{
		this->stateMachine = stateMachine;
	}

	/*void State::Delete()
	{
		delete this;
	}*/

	void State::AddTransition(int transition, State* nextState)
	{
		nextStates[transition] = nextState;
	}

	void State::SetTransition(int transition)
	{
		if (stateMachine == nullptr)
		{
			return;
		}
		stateMachine->PerformTransition(transition);
	}

}

