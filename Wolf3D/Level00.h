#pragma once
#include "GameEngine.h"
#include "Player.h"
using namespace noa;


class Level00 :public Scene
{

private:
	Level00();
	~Level00();
public:
	static Level00* Create();
	void Delete() override;

	void Awake() override;
	void Start() override;
	void Update() override;
	void Unload() override;

public:
	
	//用来释放状态机状态
	std::vector<State*> allStates;

	Player* player = nullptr;
	FreeCamera camera;
	TileMapCamera mapCamera;

	MapInfo * map = nullptr;

};

