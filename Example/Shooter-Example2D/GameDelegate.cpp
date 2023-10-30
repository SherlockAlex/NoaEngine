#include "GameDelegate.h"
#include "Player.h"
#include "BulletFactory.h"
#include "BulletPool.h"

/**
* 关于TileMap的类和结构体太多了
*/

void GameDelegate::OnLoad(noa::Scene* scene)
{

	noa::TileMap* map = noa::TileMap::Create("tileSet.tsd", { "map.csv" },scene);
	map->SetCollisionTileID({ 40 });
	noa::TileMapCamera* camera = noa::TileMapCamera::Create(scene);
	Player* player = noa::NObject<Player>::Create(scene);

	camera->SetFollow(player);
	camera->SetTileScale({32,32});

	player->camera = camera;

	bulletPool->SetFactory(bulletFactory.get());
	bulletPool->Prewarm(10);

}
