#pragma once
#include "GameEngine.h"
#include "Player.h"
using namespace noa;

class Level01 : public Scene
{
public:
	Level01();
	~Level01();

	void Awake() override;
	void Start() override;
	void Update() override;
	void Unload() override;

public:

	Player* player = nullptr;
	FreeCamera* camera = nullptr;
	TileMapCamera* mapCamera = nullptr;

	MapInfo* map = nullptr;

};

