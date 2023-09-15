#pragma once
#include "GameEngine.h"

using namespace noa;

/// <summary>
/// ��Դ�����������������Ϸ����Դ
/// </summary>
class WolfResource
{
public:
	AnimationFrame pistolFrame = AnimationFrame("./Assets/Wolf/lgun-shot.amt");
	AnimationFrame shotgunFrame = AnimationFrame("./Assets/Wolf/gun-shot.amt");

	AnimationFrame cacoIdleFrame = AnimationFrame("./Assets/Wolf/soldier-idle.amt");
	AnimationFrame cacoMoveFrame = AnimationFrame("./Assets/Wolf/soldier-walk.amt");
	AnimationFrame cacoAttckFrame = AnimationFrame("./Assets/Wolf/soldier-attack.amt");
	AnimationFrame cacoPainFrame = AnimationFrame("./Assets/Wolf/soldier-pain.amt");
	AnimationFrame cacoDieFrame = AnimationFrame("./Assets/Wolf/soldier-die.amt");

};

extern WolfResource wolfResource;

