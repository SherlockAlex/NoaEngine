#pragma once
#include "GameEngine.h"
#include "LiveEntity.h"

using namespace noa;

class Player:public Behaviour, public Rigidbody,public LiveEntity
{
public:
	Player(TileMap* map);
	~Player();
	void SetPosition(int tileID, MapFile& tileMap);
	void ActorControl();
	void RotateControl();
	void Start() override;
	void Update() override;
public:
	FreeCamera* camera = nullptr;
	float speed = 10;

	//Uint32 maxHp = 100;
	//Uint32 hp = 100;

	Sprite gunSprite = Sprite(
		LoadSprFile("./Assets/Wolf/gun.spr")
		, Vector<int>(0.5 * pixelWidth, 0.5 * pixelWidth));

	Audio shotAFX = Audio("./Assets/Wolf/Music/handleGunShot.mp3", Chunk);

	int bulletCount = 100;

	Animator* gunShot = new Animator(14);
};

