#ifndef NOAENGINE_STATE_H
#define NOAENGINE_STATE_H

#include <unordered_map>
#include "Core.h"

namespace noa {
	class NOA_API StateMachine;
	class NOA_API State
	{
	private:
		friend class StateMachine;
	protected:
		StateMachine* stateMachine;
	private:
		std::unordered_map<int, State*> nextStates;
	protected:
		State(StateMachine* stateMachine);
		virtual ~State();
	public:
		void AddTransition(int transition, State* nextState);
		void SetTransition(int transition);
	public:
		virtual void OnEnter() {};
		virtual void OnUpdate() {};
		virtual void Reason() {};
		virtual void OnExit() {};

	private:
		void Delete(State*& ptr);

	};
}

#endif

