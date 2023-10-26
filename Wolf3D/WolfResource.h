#pragma once
#include "GameEngine.h"

using namespace noa;

/// <summary>
/// 资源管理器，负责管理游戏的资源
/// </summary>
class WolfResource
{
public:
	Sprite firebomb = Sprite(Resource::LoadSprite("./Assets/Wolf/firebomb.spr"), Vector<int>(32, 32));

	AnimationClip pistolFrame = AnimationClip("./Assets/Wolf/lgun-shot.amt");
	AnimationClip shotgunFrame = AnimationClip("./Assets/Wolf/gun-shot.amt");
	AnimationClip m4a1Frame = AnimationClip("./Assets/Wolf/mgun-shot.amt");

	AnimationClip cacoIdleFrame = AnimationClip("./Assets/Wolf/soldier-idle.amt");
	AnimationClip cacoMoveFrame = AnimationClip("./Assets/Wolf/soldier-walk.amt");
	AnimationClip cacoAttckFrame = AnimationClip("./Assets/Wolf/soldier-attack.amt");
	AnimationClip cacoPainFrame = AnimationClip("./Assets/Wolf/soldier-pain.amt");
	AnimationClip cacoDieFrame = AnimationClip("./Assets/Wolf/soldier-die.amt");

	AnimationClip bulletDestroyFrame = AnimationClip("./Assets/Wolf/firebomb-destroy.amt");

	Sprite sky = Sprite(Resource::LoadSprite("./Assets/Wolf/sky-sun.spr"), Vector<int>(1, 1));

	//资源相关
	Sprite mouse = Sprite(Resource::LoadSprite("./Assets/Wolf/mouse.spr"), Vector<int>(0.02 * Screen::width, 0.02 * Screen::width));

	//SpriteGPU需要在初始化游戏引擎之后才能进行创建
	//SpriteGPU mouseGPU = SpriteGPU(&mouse);

	Sprite cacoSprite = Sprite(Vector<int>(32, 32));

	Sprite bulletSprite = Sprite(Resource::LoadSprite("./Assets/Wolf/bullet.spr"), Vector<int>(64, 64));
	Sprite healthBoxSprite = Sprite(Resource::LoadSprite("./Assets/Wolf/health-box.spr"), Vector<int>(64, 64));

	Sprite backgroundSprite = Sprite(Resource::LoadSprite("./Assets/Wolf/background.spr"), {Screen::width,Screen::height});

	std::shared_ptr<Audio> bulletPickUpSFX = std::make_shared<Audio>("./Assets/Wolf/Music/pickUpBullet.mp3", AudioType::CHUNK);

	std::shared_ptr<Audio> BGM = std::make_shared<Audio>("./Assets/Wolf/Music/theme.mp3", AudioType::MUSIC);

	std::shared_ptr<Audio> npcAttack = std::make_shared<Audio>("Assets/Wolf/Music/npc_attack.wav", AudioType::CHUNK);

	std::shared_ptr<Audio> npcDeath = std::make_shared<Audio>("Assets/Wolf/Music/npc_death.wav", AudioType::CHUNK);

	std::shared_ptr<Audio> npcPain = std::make_shared<Audio>("Assets/Wolf/Music/npc_pain.wav", AudioType::CHUNK);

};

extern std::shared_ptr<WolfResource> wolfResource;

