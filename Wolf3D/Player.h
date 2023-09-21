#pragma once
#include "GameEngine.h"
#include "LiveEntity.h"
#include "Shotgun.h"
#include "Pistol.h"
#include "M4A1.h"

using namespace noa;

class Player:public Actor,public LiveEntity
{
private:
	Player(Scene * scene);
	~Player();
public:
	static Player* Create(Scene* scene);
	void Delete() override;
	
	void SetPosition(int tileID, MapFile& tileMap);
	void ActorControl();
	void RotateControl();
	void Start() override;
	void Update() override;
	void TakeDamage(int damage) override;

	void MakeGun();

	void SetCamera(FreeCamera * camera);

public:
	Rigidbody* rigid = Rigidbody::Create(this);

	FreeCamera* camera = nullptr;
	float speed = 7;
	float mouseSpeed = 0.025;

	Audio painAFX = Audio("./Assets/Wolf/Music/player_pain.wav",Chunk);

	Audio interactAFX = Audio("./Assets/Wolf/Music/switch.wav", Chunk);

	int bulletCount = 100;

	int currentGunIndex = 0;

	Shotgun* shotgun = nullptr;
	Pistol* pistol = nullptr;
	M4A1* m4a1 = nullptr;

	vector<Gun*> guns;

};

