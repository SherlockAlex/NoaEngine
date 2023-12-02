#include "Actor.h"
#include "StateMachine.h"
#include "NoaMath.h"
#include "State.h"

#include <algorithm>

noa::StateMachine::StateMachine(noa::Actor* actor) :noa::ActorComponent(actor)
{
	currentState = nullptr;
}

noa::StateMachine::~StateMachine()
{
	currentState = nullptr;

	std::sort(stateList.begin(),stateList.end());
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
		|| currentState->nextStates.count(transition)<=0
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

