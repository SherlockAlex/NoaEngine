#include "StateMachine.h"
#include "NoaMath.h"
#include "NoaEngine.h"


noa::StateMachine::StateMachine()
{
	currentState = nullptr;
}

noa::StateMachine::StateMachine(vector<State*> stateList)
{
	this->stateList = stateList;
	if (!stateList.empty())
	{
		currentState = stateList[0];
	}
}

void noa::StateMachine::PerformTransition(int transition)
{
	//�л�����һ��״̬
	if (currentState == nullptr
		||ContainKey<int,State*>(currentState->nextStates,transition)
		) 
	{
		return;
	}
	currentState = currentState->nextStates[transition];
}

void noa::StateMachine::AddState(State* state)
{
	//���״̬
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

noa::State::State(StateMachine* stateMachine)
{
	this->stateMachine = stateMachine;
}

noa::State::~State()
{

}

void noa::State::AddTransition(int transition, State* nextState)
{
	nextStates[transition] = nextState;
}

void noa::State::SetTransition(int transition)
{
	if (!ContainKey<int,State*>(nextStates,transition))
	{
		Debug("havn't state");
	}
	stateMachine->currentState = nextStates[transition];
}
