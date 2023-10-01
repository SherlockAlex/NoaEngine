#include "Level01.h"
#include "Enimy.h"
#include "Caco.h"
#include "Item.h"
#include "WolfResource.h"

Level01::Level01() :Scene(
	"SecondFloor"
)
{
	
}

Level01::~Level01()
{
	
}

Level01* Level01::Create()
{
	return new Level01();
}

void Level01::Delete()
{
	delete this;
}

void Level01::Awake()
{
	allStates.clear();

	if (wolfResource == nullptr)
	{
		wolfResource = new WolfResource();
	}

	map = new MapInfo();
	map->mapLayer = new TileMap(
		resource.LoadTileFromTsd("./Assets/Wolf/Map/tileSet.tsd"),
		resource.LoadMapFromCSV("./Assets/Wolf/Map/第二关_图块层.csv")
	);
	map->objectLayer = new MapFile(resource.LoadMapFromCSV("./Assets/Wolf/Map/第二关_对象层.csv"));

	SetTileMap(map->mapLayer);

	player = Player::Create(this);
	camera.SetFollow(&player->transform);
	mapCamera.SetFollow(&player->transform);
	mapCamera.SetTileScale(Vector<int>(32, 32));

	player->SetCamera(&camera);

	for (int i = 0; i < map->objectLayer->w; i++)
	{
		for (int j = 0; j < map->objectLayer->h; j++)
		{
			if (map->objectLayer->image[j * map->objectLayer->w + i] == 18)
			{
				Caco* enimy = Caco::Create(this, new Sprite(), &player->transform, player);
				enimy->rigid->SetTileMap(map->mapLayer);
				enimy->item = &wolfResource->bulletSprite;
				enimy->itemPickEvent += [this]() {
					player->bulletCount += 10;
					wolfResource->bulletPickUpSFX->Play(false);
					};
				enimy->transform.position.x = i;
				enimy->transform.position.y = j;
				enimy->transform.posZ = 0;

			}
		}
	}

	//场景生成物品
	for (int i = 0; i < map->objectLayer->w; i++)
	{
		for (int j = 0; j < map->objectLayer->h; j++)
		{
			if (map->objectLayer->image[j * map->objectLayer->w + i] == 27)
			{
				Item* healthBox = Item::Create(this, &wolfResource->healthBoxSprite);
				healthBox->rigid->SetTileMap(map->mapLayer);
				healthBox->transform.position.x = i;
				healthBox->transform.position.y = j;

				healthBox->pickEvent += [this]() {
					player->hp += 5;
					if (player->hp > player->maxHp)
					{
						player->hp = player->maxHp;
					}
					wolfResource->bulletPickUpSFX->Play(false);
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
	player->rigid->SetTileMap(map->mapLayer);
	//设置玩家位置
	player->SetPosition(35, *map->objectLayer);
	player->transform.eulerAngle = PI;
}

void Level01::Start()
{
	wolfResource->BGM->volume = 0.4;
	wolfResource->BGM->Play(true);
	inputSystem.SetRelativeMouseMode(true);
}

void Level01::Update()
{
	camera.Render(*map->mapLayer, false, nullptr, WHITE);
	wolfResource->mouseGPU.DrawSprite(pixelWidth * 0.5 - 0.5 * wolfResource->mouse.scale.x, pixelHeight * 0.5 - 0.5 * wolfResource->mouse.scale.y, true);
	if (inputSystem.GetKeyHold(KEYCODE::KeyM))
	{
		Vector<int> drawPos = mapCamera.Render(*map->mapLayer, { 0,0 }, { 0,0 });
		renderer->DrawRect(drawPos, drawPos + Vector<int>(32, 32), WHITE);
	}

}

void Level01::Unload()
{
	if (map != nullptr)
	{
		delete map;
		Debug("卸载场景成功");
	}

	if (!allStates.empty()) 
	{
		auto last = std::unique(allStates.begin(), allStates.end());
		allStates.erase(last, allStates.end());

		for (int i = 0; i < allStates.size(); i++)
		{
			if (allStates[i] == nullptr)
			{
				continue;
			}
			allStates[i]->Delete();
			//allStates[i] = nullptr;
		}

	}

	allStates.clear();

	

}
