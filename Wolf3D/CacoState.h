#pragma once
#include "GameEngine.h"

using namespace noa;


class Enimy;

enum EnimyState
{
	Idle, Move, Attack, Die,Pain
};


class CacoIdleState :public State
{
private:
	Enimy* enimy = nullptr;
	Transform* target = nullptr;
	

public:
	Animation * animation = Animation::Create(5,false);

	CacoIdleState(
		StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationFrame * frameData
	);

	void OnUpdate() override;
	void Reason() override;

};

class CacoMoveState :public State
{
private:
	Enimy* enimy = nullptr;
	Transform* target = nullptr;
	

public:
	Animation * animation = Animation::Create(5,false);

	CacoMoveState(
		StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationFrame* frameData
	);

	void OnUpdate() override;
	void Reason() override;

};

class CacoAttackState :public State
{
public:
	Enimy* enimy = nullptr;
	Transform* target = nullptr;
	Audio audio = Audio("Assets/Wolf/Music/npc_attack.wav", Chunk);

public:
	Animation * animation = Animation::Create(5,false);

	CacoAttackState(
		StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationFrame* frameData
	);

	void OnUpdate() override;
	void Reason() override;

};


class CacoDieState :public State
{
public:
	Enimy* enimy = nullptr;
	Transform* target = nullptr;
	Audio audio = Audio("Assets/Wolf/Music/npc_death.wav", Chunk);

public:
	Animation* animation = Animation::Create(12,false);

	CacoDieState(
		StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationFrame* frameData
	);

	void OnEnter() override;
	void OnUpdate() override;
	void Reason() override;

};

class CacoPainState :public State
{
public:
	Enimy* enimy = nullptr;
	Transform* target = nullptr;

	Audio audio = Audio("Assets/Wolf/Music/npc_pain.wav", Chunk);

public:
	Animation *animation = Animation::Create(5, false);

	CacoPainState(
		StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationFrame* frameData
	);

	void OnUpdate() override;
	void Reason() override;

};

