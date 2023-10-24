#include "Actor.h"
#include "StateMachine.h"
#include "NoaMath.h"

#include <algorithm>

namespace noa {

	

}

noa::StateMachine::StateMachine(noa::Actor* actor) :noa::ActorComponent(actor)
{
	currentState = nullptr;
}

noa::StateMachine::~StateMachine()
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

noa::StateMachine* noa::StateMachine::Create(noa::Actor* actor)
{
	return new noa::StateMachine(actor);
}

void noa::StateMachine::PerformTransition(int transition)
{
	const bool checkNextStateError = currentState == nullptr
		|| !ContainKey<int, noa::State*>(currentState->nextStates, transition)
		|| currentState->nextStates[transition] == nullptr;
	if (checkNextStateError)
	{
		return;
	}

	currentState->OnExit();
	currentState = currentState->nextStates[transition];
	currentState->OnEnter();

}

void noa::StateMachine::AddState(noa::State* state)
{
	//Ìí¼Ó×´Ì¬
	stateList.push_back(state);
	if (currentState == nullptr)
	{
		currentState = state;
	}
}

void noa::StateMachine::Act()
{
	if (currentState == nullptr)
	{
		return;
	}
	currentState->OnUpdate();
}

void noa::StateMachine::Reason()
{
	if (currentState == nullptr)
	{
		return;
	}
	currentState->Reason();
}

void noa::StateMachine::Update()
{
	if (currentState == nullptr)
	{
		return;
	}
	currentState->Reason();
	currentState->OnUpdate();
}

noa::State::State(noa::StateMachine* stateMachine)
{
	this->stateMachine = stateMachine;
}

noa::State::~State() {

}


void noa::State::AddTransition(int transition, noa::State* nextState)
{
	nextStates[transition] = nextState;
}

void noa::State::SetTransition(int transition)
{
	if (stateMachine == nullptr)
	{
		return;
	}
	stateMachine->PerformTransition(transition);
}

void noa::State::Delete(noa::State*& ptr)
{
	delete this;
	ptr = nullptr;
}


