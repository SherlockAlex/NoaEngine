#include "Level01.h"
#include "Enimy.h"
#include "Caco.h"
#include "Item.h"
#include "WolfResource.h"
#include "Enter.h"

static Player* player = nullptr;
static FreeCamera* camera = nullptr;
static TileMapCamera* mapCamera = nullptr;
static std::shared_ptr<MapInfo> map = nullptr;

void Level01Delegate::OnLoad(Scene * scene)
{

	map = std::make_shared<MapInfo>();
	map->mapLayer = std::make_shared<TileMap>(
		Resource::LoadTileSet("Map/tileSet.tsd"),
		Resource::LoadTileMap("Map/第二关_图块层.csv")
	);
	map->objectLayer = std::make_shared<MapFile>(Resource::LoadTileMap("Map/第二关_对象层.csv"));

	scene->SetTileMap(map->mapLayer.get());

	player = Player::Create(scene);

	camera = NObject<FreeCamera>::Create<Scene*>(scene);
	mapCamera = NObject<TileMapCamera>::Create<Scene*>(scene);

	camera->SetFollow(&player->transform);
	camera->SetTileMap(map->mapLayer.get());

	mapCamera->SetFollow(&player->transform);
	mapCamera->SetTileScale(Vector<int>(32, 32));

	player->SetCamera(camera);

	for (int i = 0; i < map->objectLayer->w; i++)
	{
		for (int j = 0; j < map->objectLayer->h; j++)
		{
			const int tile = map->objectLayer->image[j * map->objectLayer->w + i];
			if (tile == 18)
			{
				Caco* enimy = Caco::Create(scene, new Sprite(), &player->transform, player);
				enimy->rigid->SetTileMap(map->mapLayer.get());
				enimy->item = &wolfResource->bulletSprite;
				enimy->itemPickEvent += [scene]() {
					player->bulletCount += 10;
					//wolfResource->bulletPickUpSFX->Play(false);
					};
				enimy->transform.position.x = i;
				enimy->transform.position.y = j;
				enimy->transform.posZ = 0;

			}
			else if (tile == 102)
			{

			}

		}
	}

	Enter* enter =
		NObject<Enter>::Create<Scene*, const std::string&>(
			scene, "NewGame"
		);

	//场景生成物品
	for (int i = 0; i < map->objectLayer->w; i++)
	{
		for (int j = 0; j < map->objectLayer->h; j++)
		{
			if (map->objectLayer->image[j * map->objectLayer->w + i] == 27)
			{
				Item* healthBox = Item::Create(scene, &wolfResource->healthBoxSprite);
				healthBox->rigid->SetTileMap(map->mapLayer.get());
				healthBox->transform.position.x = i;
				healthBox->transform.position.y = j;
				healthBox->SetInteractAudioClip(wolfResource->bulletPickUpSFX);
				healthBox->pickEvent += [scene]() {
					player->hp += 5;
					if (player->hp > player->maxHp)
					{
						player->hp = player->maxHp;
					}
					//wolfResource->bulletPickUpSFX->Play(false);
					};

			}
		}
	}

	//设置场景的碰撞区域
	std::vector<int> collisionTileID;
	collisionTileID.push_back(36);
	for (int i = 0; i < 108; i++)
	{
		if (i == 107)
		{
			continue;
		}

		collisionTileID.push_back(i);

	}
	map->mapLayer->SetCollisionTileID(collisionTileID);

	//传递当前场景地图给玩家
	player->rigid->SetTileMap(map->mapLayer.get());
	//设置玩家位置
	player->SetPosition(35, *map->objectLayer);
	player->transform.eulerAngle = PI;

	inputSystem.SetRelativeMouseMode(true);
}

