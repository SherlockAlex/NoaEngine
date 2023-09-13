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
	void TakeDamage(int damage) override;
public:
	FreeCamera* camera = nullptr;
	float speed = 10;

	//Uint32 maxHp = 100;
	//Uint32 hp = 100;

	Sprite gunSprite = Sprite(
		resource.LoadSprFile("./Assets/Wolf/gun.spr")
		, Vector<int>(0.25 * pixelWidth, 0.25 * pixelWidth));

	Audio shotAFX = Audio("./Assets/Wolf/Music/shotgun.wav", Chunk);
	Audio painAFX = Audio("./Assets/Wolf/Music/player_pain.wav",Chunk);

	int bulletCount = 100;

	Animation* gunShot = new Animation(7.5,false);
};

