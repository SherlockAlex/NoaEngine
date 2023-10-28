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
	Animation * animation = nullptr;
	AudioSource* audio = nullptr;
private:

	CacoIdleState(
		StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationClip* frameData
	);

public:
	static CacoIdleState* Create(StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationClip* frameData) {
		return new CacoIdleState(stateMachine,enimy,target,frameData);
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
	Animation* animation = nullptr;
	AudioSource* audio = nullptr;
private:

	CacoMoveState(
		StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationClip* frameData
	);

public:
	static CacoMoveState* Create(StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationClip* frameData) {
		return new CacoMoveState(stateMachine, enimy, target, frameData);
	}

	void OnUpdate() override;
	void Reason() override;

};

class CacoAttackState :public State
{
public:
	Enimy* enimy = nullptr;
	Transform* target = nullptr;
	

public:
	Animation* animation = nullptr;
	AudioSource* audio = nullptr;
private:

	CacoAttackState(
		StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationClip* frameData
	);

public:
	static CacoAttackState* Create(StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationClip* frameData) {
		return new CacoAttackState(stateMachine, enimy, target, frameData);
	}

	void OnUpdate() override;
	void Reason() override;

};


class CacoDieState :public State
{
public:
	Enimy* enimy = nullptr;
	Transform* target = nullptr;
	

public:
	Animation* animation = nullptr;
	AudioSource* audio = nullptr;
private:

	CacoDieState(
		StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationClip* frameData
	);

public:
	static CacoDieState* Create(StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationClip* frameData) {
		return new CacoDieState(stateMachine, enimy, target, frameData);
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

	

public:
	Animation* animation = nullptr;
	AudioSource* audio = nullptr;
private:

	CacoPainState(
		StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationClip* frameData
	);

public:
	static CacoPainState* Create(StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		AnimationClip* frameData) {
		return new CacoPainState(stateMachine, enimy, target, frameData);
	}

	void OnUpdate() override;
	void Reason() override;

};

