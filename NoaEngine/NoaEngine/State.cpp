#include "State.h"
#include "StateMachine.h"

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
