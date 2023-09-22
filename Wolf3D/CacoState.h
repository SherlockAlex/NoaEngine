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

public:
	CacoIdleState(
		StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationFrame * frameData
	);

	~CacoIdleState() override
	{
		State::~State();
		Debug("Remove Caco state successfully !!!!!!!");
	}
public:
	static CacoIdleState* Create(StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationFrame* frameData) {
		return new CacoIdleState(stateMachine,enimy,target,frameData);
	}

	void Delete() override {
		delete this;
	}

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

public:
	CacoMoveState(
		StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationFrame* frameData
	);
	~CacoMoveState() override
	{
		State::~State();
		Debug("Remove Caco state successfully !!!!!!!");
	}

public:
	static CacoMoveState* Create(StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationFrame* frameData) {
		return new CacoMoveState(stateMachine, enimy, target, frameData);
	}

	void Delete() override {
		delete this;
	}

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

public:
	CacoAttackState(
		StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationFrame* frameData
	);
	~CacoAttackState() override
	{
		State::~State();
		Debug("Remove Caco state successfully !!!!!!!");
	}

public:
	static CacoAttackState* Create(StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationFrame* frameData) {
		return new CacoAttackState(stateMachine, enimy, target, frameData);
	}

	void Delete() override {
		delete this;
	}

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

public:
	CacoDieState(
		StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationFrame* frameData
	);
	~CacoDieState() override
	{
		State::~State();
		Debug("Remove Caco state successfully !!!!!!!");
	}

public:
	static CacoDieState* Create(StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationFrame* frameData) {
		return new CacoDieState(stateMachine, enimy, target, frameData);
	}

	void Delete() override {
		delete this;
	}

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

public:
	CacoPainState(
		StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationFrame* frameData
	);
	~CacoPainState() override
	{
		State::~State();
		Debug("Remove Caco state successfully !!!!!!!");
	}

public:
	static CacoPainState* Create(StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationFrame* frameData) {
		return new CacoPainState(stateMachine, enimy, target, frameData);
	}

	void Delete() override {
		delete this;
	}

	void OnUpdate() override;
	void Reason() override;

};

