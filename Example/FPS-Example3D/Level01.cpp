#include "Level01.h"
#include "Enimy.h"
#include "Caco.h"
#include "Item.h"
#include "WolfResource.h"
#include "Enter.h"

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

void Level01::Awake()
{

	map = std::make_shared<MapInfo>();
	map->mapLayer = std::make_shared<TileMap>(
		Resource::LoadTileSet("Map/tileSet.tsd"),
		Resource::LoadTileMap("Map/�ڶ���_ͼ���.csv")
	);
	map->objectLayer = std::make_shared<MapFile>(Resource::LoadTileMap("Map/�ڶ���_�����.csv"));

	SetTileMap(map->mapLayer.get());

	player = Player::Create(this);

	camera = NObject<FreeCamera>::Create<Scene*>(this);
	mapCamera = NObject<TileMapCamera>::Create<Scene*>(this);

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
				Caco* enimy = Caco::Create(this, new Sprite(), &player->transform, player);
				enimy->rigid->SetTileMap(map->mapLayer.get());
				enimy->item = &wolfResource->bulletSprite;
				enimy->itemPickEvent += [this]() {
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
			this, "NewGame"
		);

	//����������Ʒ
	for (int i = 0; i < map->objectLayer->w; i++)
	{
		for (int j = 0; j < map->objectLayer->h; j++)
		{
			if (map->objectLayer->image[j * map->objectLayer->w + i] == 27)
			{
				Item* healthBox = Item::Create(this, &wolfResource->healthBoxSprite);
				healthBox->rigid->SetTileMap(map->mapLayer.get());
				healthBox->transform.position.x = i;
				healthBox->transform.position.y = j;

				healthBox->pickEvent += [this]() {
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

	//���ó�������ײ����
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

	//���ݵ�ǰ������ͼ�����
	player->rigid->SetTileMap(map->mapLayer.get());
	//�������λ��
	player->SetPosition(35, *map->objectLayer);
	player->transform.eulerAngle = PI;
}

void Level01::Start()
{
	//wolfResource->BGM->volume = 0.4;
	//wolfResource->BGM->Play(true);
	inputSystem.SetRelativeMouseMode(true);
}

void Level01::Update()
{

}

void Level01::Unload()
{

}
