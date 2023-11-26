#ifndef NOAENGINE_STATEMACHINE
#define NOAENGINE_STATEMACHINE

#include <vector>
#include <unordered_map>
#include <memory>
#include "Debug.h"

#include "ActorComponent.h"

/*
* ��״̬�����Զ�������һ���Ż���ע�⵽״̬����Reason()��Act(),һȺС�ֿ��ܳ��е�״̬������ͬ�ģ���ͬ�ĵط���������������,
* ״̬�������ṩ����ķ������̣���������������С��
*/

namespace noa 
{
	class State;

	class StateMachine:ActorComponent
	{
		friend class NObject<StateMachine>;
	protected:
		std::vector<State*> stateList;
	public:
		State* currentState = nullptr;
	protected:
		StateMachine(Actor * actor);
		virtual ~StateMachine() override;

	public:
		static StateMachine* Create(Actor* actor);
		virtual void PerformTransition(int transition);
		virtual void Act();
		virtual void Reason();
		virtual void Update() override;
		virtual void AddState(State* state);

	};
}

#endif // !NOAENGINE_STATEMACHINE


