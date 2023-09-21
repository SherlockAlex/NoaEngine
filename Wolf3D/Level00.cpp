#include "Level00.h"
#include "Enimy.h"
#include "Caco.h"
#include "Item.h"

Level00::Level00():Scene(
	"NewGame",
	{
		"./Assets/Wolf/Map/新手关_图块层 1.csv" ,
		"./Assets/Wolf/Map/新手关_对象层.csv",
		"./Assets/Wolf/Map/tileSet.tsd"
	}
)
{
	map = new MapInfo(this->GetTileMap());
}

Level00::~Level00()
{
	Scene::~Scene();
}

void Level00::Awake()
{
	player = Player::Create(this);
	camera = new FreeCamera(&player->transform);
	mapCamera = new TileMapCamera(Vector<int>(32, 32), &player->transform);

	player->SetCamera(camera);

	for (int i = 0; i < map->objectLayer.w; i++)
	{
		for (int j = 0; j < map->objectLayer.h; j++)
		{
			if (map->objectLayer.image[j * map->objectLayer.w + i] == 18)
			{
				Caco * enimy = Caco::Create(this,new Sprite(), &player->transform, player);
				enimy->rigid->SetTileMap(&map->mapLayer);
				enimy->item = &bulletSprite;
				enimy->itemPickEvent += [this]() {
					player->bulletCount += 10;
					bulletPickUpSFX.Play(false);
					};
				enimy->transform.position.x = i;
				enimy->transform.position.y = j;
				enimy->transform.posZ = 0;

			}
		}
	}

	//场景生成物品
	for (int i = 0; i < map->objectLayer.w; i++)
	{
		for (int j = 0; j < map->objectLayer.h; j++)
		{
			if (map->objectLayer.image[j * map->objectLayer.w + i] == 27)
			{
				Item * healthBox = Item::Create(this,&healthBoxSprite);
				healthBox->rigid->SetTileMap(&map->mapLayer);
				healthBox->transform.position.x = i;
				healthBox->transform.position.y = j;

				healthBox->pickEvent += [this]() {
					player->hp += 5;
					if (player->hp > player->maxHp)
					{
						player->hp = player->maxHp;
					}
					bulletPickUpSFX.Play(false);
					};

			}
		}
	}

	//设置场景的碰撞区域
	vector<int> collisionTileID;
	collisionTileID.push_back(36);
	for (int i = 0; i < 108; i++)
	{
		if (i == 107)
		{
			continue;
		}

		collisionTileID.push_back(i);

	}
	map->mapLayer.SetCollisionTileID(collisionTileID);

	//传递当前场景地图给玩家
	player->rigid->SetTileMap(&map->mapLayer);
	//设置玩家位置
	player->SetPosition(35, map->objectLayer);
	player->transform.eulerAngle = PI;
}

void Level00::Start()
{
	BGM.volume = 0.4;
	BGM.Play(true);
	inputSystem.SetRelativeMouseMode(true);
}

void Level00::Update()
{
	
	camera->Render(map->mapLayer, false, nullptr, WHITE);
	mouse.DrawSprite(pixelWidth * 0.5 - 0.5 * mouse.scale.x, pixelHeight * 0.5 - 0.5 * mouse.scale.y, true);
	if (inputSystem.GetKeyHold(KeyM))
	{
		Vector<int> drawPos = mapCamera->Render(map->mapLayer, { 0,0 }, {0,0});
		renderer.DrawRect(drawPos, drawPos + Vector<int>(32, 32), WHITE);
	}

	
}

void Level00::Unload()
{

}
