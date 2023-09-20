#pragma once
#include "GameEngine.h"
#include "Player.h"
using namespace noa;


class Level00 :public Scene
{
public:

	Level00();
	~Level00();

	void Awake() override;
	void Start() override;
	void Update() override;
	void Unload() override;

public:
	Sprite sky = Sprite(resource.LoadSprFile("./Assets/Wolf/sky-sun.spr"), Vector<int>(1.0, 1.0));

	Player* player = nullptr;
	FreeCamera* camera = nullptr;
	TileMapCamera* mapCamera = nullptr;

	//资源相关
	Sprite mouse = Sprite(resource.LoadSprFile("./Assets/Wolf/mouse.spr"), Vector<int>(0.03 * pixelWidth, 0.03 * pixelWidth));

	//Sprite cacoSprite = Sprite(resource.LoadSprFile("./Assets/Wolf/caco.spr"), Vector<int>(32, 32));
	Sprite cacoSprite = Sprite(Vector<int>(32, 32));

	Sprite bulletSprite = Sprite(resource.LoadSprFile("./Assets/Wolf/bullet.spr"), Vector<int>(64, 64));
	Sprite healthBoxSprite = Sprite(resource.LoadSprFile("./Assets/Wolf/health-box.spr"), Vector<int>(64, 64));

	Audio bulletPickUpSFX = Audio("./Assets/Wolf/Music/pickUpBullet.mp3", Chunk);

	Audio BGM = Audio("./Assets/Wolf/Music/theme.mp3", Music);

	MapInfo * map = nullptr;

};

