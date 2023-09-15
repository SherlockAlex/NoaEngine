#pragma once
#include "GameEngine.h"
#include "LiveEntity.h"
#include "Shotgun.h"
#include "Pistol.h"

using namespace noa;

class Player:public Behaviour, public Rigidbody,public LiveEntity
{
public:
	Player();
	~Player();
	void SetPosition(int tileID, MapFile& tileMap);
	void ActorControl();
	void RotateControl();
	void Start() override;
	void Update() override;
	void TakeDamage(int damage) override;

	void MakeGun();

	void SetCamera(FreeCamera * camera);

public:
	FreeCamera* camera = nullptr;
	float speed = 7;
	float mouseSpeed = 0.025;

	Audio painAFX = Audio("./Assets/Wolf/Music/player_pain.wav",Chunk);

	int bulletCount = 100;

	int currentGunIndex = 0;

	Shotgun* shotgun = nullptr;
	Pistol* pistol = nullptr;

	vector<Gun*> guns;

};

