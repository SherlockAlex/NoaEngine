#include "GameDelegate.h"
#include "Player.h"
#include "BulletFactory.h"
#include "BulletPool.h"

/**
* 关于TileMap的类和结构体太多了
*/

std::shared_ptr<noa::MapInfo> map = nullptr;
void GameDelegate::OnLoad(noa::Scene* scene)
{
	map = std::make_shared<noa::MapInfo>();
	map->mapLayer = std::make_shared<noa::TileMap>(
		noa::Resource::LoadTileSet("tileSet.tsd"),
		noa::Resource::LoadTileMap("map.csv")
	);
	map->mapLayer->SetCollisionTileID({ 40 });

	scene->SetTileMap(map->mapLayer.get());
	


	noa::TileMapCamera* camera = noa::TileMapCamera::Create(scene);

	Player* player = noa::NObject<Player>::Create(scene);

	player->rigid->SetTileMap(map->mapLayer.get());

	camera->SetFollow(player);
	camera->SetTileMap(map->mapLayer.get());
	camera->SetTileScale({32,32});

	player->camera = camera;

	bulletPool->SetFactory(bulletFactory.get());
	bulletPool->Prewarm(10);

}
