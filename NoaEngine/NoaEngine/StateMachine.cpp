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
		
		auto last = std::unique(stateList.begin(), stateList.end());
		stateList.erase(last, stateList.end());

		for (auto& state : stateList)
		{
			if (state == nullptr)
			{
				continue;
			}
			state->Delete(state);
		}

		stateList.clear();
	}

	StateMachine* StateMachine::Create(Actor* actor)
	{
		return new StateMachine(actor);
	}

	void StateMachine::PerformTransition(int transition)
	{
		const bool checkNextStateError = currentState == nullptr
			|| !ContainKey<int, State*>(currentState->nextStates, transition)
			|| currentState->nextStates[transition] == nullptr;
		if (checkNextStateError)
		{
			return;
		}

		currentState->OnExit();
		currentState = currentState->nextStates[transition];
		currentState->OnEnter();

	}

	void StateMachine::AddState(State* state)
	{
		//Ìí¼Ó×´Ì¬
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

	State::~State() {

	}


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

	void State::Delete(State*& ptr)
	{
		delete this;
		ptr = nullptr;
	}

}

