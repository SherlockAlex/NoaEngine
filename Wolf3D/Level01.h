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
	void Delete() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void Unload() override;

public:

	std::vector<State*> allStates;

	Player* player = nullptr;
	FreeCamera camera;
	TileMapCamera mapCamera;

	MapInfo* map = nullptr;

};

