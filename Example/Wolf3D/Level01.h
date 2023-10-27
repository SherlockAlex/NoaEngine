#pragma once
#include "GameEngine.h"
#include "Player.h"
using namespace noa;

class Level01 : public Scene
{
private:
	Level01();
	~Level01();
public:
	static Level01* Create();

	void Awake() override;
	void Start() override;
	void Update() override;
	void Unload() override;

public:
	Player* player = nullptr;
	FreeCamera* camera = nullptr;
	TileMapCamera* mapCamera = nullptr;

	std::shared_ptr<MapInfo> map = nullptr;

};

