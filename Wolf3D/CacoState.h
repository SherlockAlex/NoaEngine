#pragma once
#include "GameEngine.h"

using namespace noa;


class Enimy;

enum EnimyState
{
	Idle, Move, Attack, Die
};


class CacoIdleState :public State
{
private:
	Enimy* enimy = nullptr;
	Transform* target = nullptr;
	

public:
	Animation animation = Animation(5,false);

	CacoIdleState(
		StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		const char * animationPath
	);

	void Act() override;
	void Reason() override;

};

class CacoMoveState :public State
{
private:
	Enimy* enimy = nullptr;
	Transform* target = nullptr;
	

public:
	Animation animation = Animation(5,false);

	CacoMoveState(
		StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		const char* animationPath);

	void Act() override;
	void Reason() override;

};

class CacoAttackState :public State
{
public:
	Enimy* enimy = nullptr;
	Transform* target = nullptr;
	

public:
	Animation animation = Animation(5,false);

	CacoAttackState(
		StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		const char* animationPath);

	void Act() override;
	void Reason() override;

};


class CacoDieState :public State
{
public:
	Enimy* enimy = nullptr;
	Transform* target = nullptr;
	

public:
	Animation animation = Animation(5,false);

	CacoDieState(
		StateMachine* stateMachine,
		Enimy* enimy, Transform* target,
		const char* animationPath);

	void Act() override;
	void Reason() override;

};

