#pragma once
#include "GameEngine.h"

using namespace noa;

/// <summary>
/// 资源管理器，负责管理游戏的资源
/// </summary>
class WolfResource
{
public:
	Sprite firebomb = Sprite(resource.LoadSprFile("./Assets/Wolf/firebomb.spr"), Vector<int>(32, 32));

	AnimationFrame pistolFrame = AnimationFrame("./Assets/Wolf/lgun-shot.amt");
	AnimationFrame shotgunFrame = AnimationFrame("./Assets/Wolf/gun-shot.amt");
	AnimationFrame m4a1Frame = AnimationFrame("./Assets/Wolf/mgun-shot.amt");

	AnimationFrame cacoIdleFrame = AnimationFrame("./Assets/Wolf/soldier-idle.amt");
	AnimationFrame cacoMoveFrame = AnimationFrame("./Assets/Wolf/soldier-walk.amt");
	AnimationFrame cacoAttckFrame = AnimationFrame("./Assets/Wolf/soldier-attack.amt");
	AnimationFrame cacoPainFrame = AnimationFrame("./Assets/Wolf/soldier-pain.amt");
	AnimationFrame cacoDieFrame = AnimationFrame("./Assets/Wolf/soldier-die.amt");

};

extern WolfResource wolfResource;

