#include "GameDelegate.h"
#include "Player.h"
#include "BulletFactory.h"
#include "BulletPool.h"
#include "Test.h"
#include "GameUI.h"
#include "GameMenu.h"

bool flag = true;


void GameMenuScene(noa::Scene* scene);
void GameDelegate::OnLoad(noa::Scene* scene)
{

	noa::PhysicsSystem::SetGrid(100,100);

	noa::TileMapInfo mapInfo = noa::TileMapInfo()
		.LoadTileSet("tileSet.tsd")
		.LoadTileLayer(
			{"map_Layer1.csv","map_Layer2.csv"}
		);

	noa::TileMap* tileMap = noa::TileMap::Create(scene);
	noa::TileMapRenderer* tileMapRenderer 
		= noa::TileMapRenderer::Create(tileMap)
		->SetTileMap(mapInfo.layers[0], mapInfo.GetTileSet())
		.SetCollision(40)
		.Apply();
	noa::TileMapRenderer* tileMapRenderer1
		= noa::TileMapRenderer::Create(tileMap)
		->SetTileMap(mapInfo.layers[1], mapInfo.GetTileSet())
		.SetOffset(10,8)
		.SetCollision(84)
		.Apply();

	Test* test1 = noa::NObject<Test>::Create(scene);
	test1->tileCollider->SetTileMap(tileMap);
	test1->transform.position = { 2,3 };

	Player* player = noa::NObject<Player>::Create(scene);
	player->tileCollider->SetTileMap(tileMap);

	noa::Camera2D* camera = noa::Camera2D::Create(scene)
		->SetFar(32.0f)
		.SetAnchor(0.5f,0.5f)
		.SetBoundary(true)
		.SetHorizonalBoundary(0, mapInfo.w)
		.SetVerticalBoundary(0,mapInfo.h)
		.SetFollow(player)
		.Apply();

	bulletFactory->SetActiveScene(scene);
	bulletPool = std::make_shared<noa::ActorPool<Bullet>>();
	bulletPool->SetFactory(bulletFactory.get());
	bulletPool->Prewarm(10);

	GameUI* gui = noa::NObject<GameUI>::Create(scene);

	noa::Scene* menuScene = scene->CreateChild("MenuScene");
	GameMenuScene(menuScene);

}

GameMenu* gameMenu = nullptr;
void GameDelegate::OnTick(noa::Scene* scene)
{

	if (gameMenu == nullptr)
	{
		return;
	}

	if (noa::Input::GetKeyUp(noa::KeyCode::KEY_ESC)) 
	{
		flag = !flag;
		if (flag)
		{
			gameMenu->Show();
			scene->ActiveSceneChild("MenuScene");
		}
		else {
			gameMenu->Close();
			scene->CloseSceneChild();
		}
	}

	

}


void GameMenuScene(noa::Scene* scene) {
	//创建游戏菜单
	gameMenu = noa::NObject<GameMenu>::Create(scene);
}