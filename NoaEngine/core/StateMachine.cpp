#include "StateMachine.h"

void noa::StateMachine::PerformTransition(Transition transition)
{
	if (currentState == nullptr) 
	{
		return;
	}
	currentState = currentState->nextStates[transition];
}

void noa::StateMachine::Act()
{
	if (currentState == nullptr)
	{
		return;
	}
	currentState->Act();
}

void noa::StateMachine::Reason()
{
	if (currentState == nullptr)
	{
		return;
	}
	currentState->Reason();
}

noa::State::State()
{
}

noa::State::~State()
{

}
