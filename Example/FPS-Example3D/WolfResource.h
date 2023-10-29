#pragma once
#include "GameEngine.h"

using namespace noa;

/// <summary>
/// 资源管理器，负责管理游戏的资源
/// </summary>
class WolfResource
{
public:
	Sprite firebomb = Sprite(Resource::LoadSprite("firebomb.spr"), Vector<int>(32, 32));

	AnimationClip pistolFrame = AnimationClip("lgun-shot.amt");
	AnimationClip shotgunFrame = AnimationClip("gun-shot.amt");
	AnimationClip m4a1Frame = AnimationClip("mgun-shot.amt");

	AnimationClip cacoIdleFrame = AnimationClip("soldier-idle.amt");
	AnimationClip cacoMoveFrame = AnimationClip("soldier-walk.amt");
	AnimationClip cacoAttckFrame = AnimationClip("soldier-attack.amt");
	AnimationClip cacoPainFrame = AnimationClip("soldier-pain.amt");
	AnimationClip cacoDieFrame = AnimationClip("soldier-die.amt");

	AnimationClip bulletDestroyFrame = AnimationClip("firebomb-destroy.amt");

	Sprite defaultButtonImage = Sprite("btn-white.spr",310,310);

	Sprite sky = Sprite(Resource::LoadSprite("sky-sun.spr"), Vector<int>(1, 1));

	//资源相关
	Sprite mouse = Sprite(Resource::LoadSprite("mouse.spr"), Vector<int>(0.02 * Screen::width, 0.02 * Screen::width));

	//SpriteGPU需要在初始化游戏引擎之后才能进行创建
	//SpriteGPU mouseGPU = SpriteGPU(&mouse);

	Sprite cacoSprite = Sprite(Vector<int>(32, 32));

	Sprite bulletSprite = Sprite("bullet.spr",64, 64);
	Sprite healthBoxSprite = Sprite("health-box.spr",64, 64);

	Sprite backgroundSprite = Sprite("background.spr", Screen::width,Screen::height);

	std::shared_ptr<AudioClip> playerPainSFX = Resource::LoadAudioClip("Music/player_pain.wav");
	std::shared_ptr<AudioClip> playerInteractAFX = Resource::LoadAudioClip("Music/switch.wav");


	std::shared_ptr<AudioClip> pistolShotSFX = Resource::LoadAudioClip("Music/p228-1.wav");
	std::shared_ptr<AudioClip> shotgunShotSFX = Resource::LoadAudioClip("Music/sg552-2.wav");
	std::shared_ptr<AudioClip> m4a1ShotSFX = Resource::LoadAudioClip("Music/m4a1.wav");

	std::shared_ptr<AudioClip> bulletPickUpSFX = Resource::LoadAudioClip("Music/pickUpBullet.mp3");

	std::shared_ptr<AudioClip> BGM = Resource::LoadAudioClip("/Music/theme.mp3");

	std::shared_ptr<AudioClip> npcAttack = Resource::LoadAudioClip("Music/npc_attack.wav");

	std::shared_ptr<AudioClip > npcDeath = Resource::LoadAudioClip("Music/npc_death.wav");

	std::shared_ptr<AudioClip> npcPain = Resource::LoadAudioClip("Music/npc_pain.wav");

};

extern std::shared_ptr<WolfResource> wolfResource;

