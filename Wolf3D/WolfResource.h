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

	AnimationFrame bulletDestroyFrame = AnimationFrame("./Assets/Wolf/firebomb-destroy.amt");

	Sprite sky = Sprite(resource.LoadSprFile("./Assets/Wolf/sky-sun.spr"), Vector<int>(1, 1));

	//资源相关
	Sprite mouse = Sprite(resource.LoadSprFile("./Assets/Wolf/mouse.spr"), Vector<int>(0.02 * pixelWidth, 0.02 * pixelWidth));

	SpriteGPU mouseGPU = SpriteGPU(&mouse);

	//Sprite cacoSprite = Sprite(resource.LoadSprFile("./Assets/Wolf/caco.spr"), Vector<int>(32, 32));
	Sprite cacoSprite = Sprite(Vector<int>(32, 32));

	Sprite bulletSprite = Sprite(resource.LoadSprFile("./Assets/Wolf/bullet.spr"), Vector<int>(64, 64));
	Sprite healthBoxSprite = Sprite(resource.LoadSprFile("./Assets/Wolf/health-box.spr"), Vector<int>(64, 64));

	Sprite backgroundSprite = Sprite(resource.LoadSprFile("./Assets/Wolf/background.spr"), {pixelWidth,pixelHeight});

	Audio * bulletPickUpSFX = new Audio("./Assets/Wolf/Music/pickUpBullet.mp3", AudioType::CHUNK);

	Audio * BGM = new Audio("./Assets/Wolf/Music/theme.mp3", AudioType::MUSIC);

	Audio * npcAttack = new Audio("Assets/Wolf/Music/npc_attack.wav", AudioType::CHUNK);

	Audio * npcDeath = new Audio("Assets/Wolf/Music/npc_death.wav", AudioType::CHUNK);

	Audio * npcPain = new Audio("Assets/Wolf/Music/npc_pain.wav", AudioType::CHUNK);

};

extern WolfResource * wolfResource;

