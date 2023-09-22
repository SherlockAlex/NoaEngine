#include "Level00.h"
#include "Enimy.h"
#include "Caco.h"
#include "Item.h"
#include "WolfResource.h"

Level00::Level00():Scene(
	"NewGame"
)
{
	
}

Level00::~Level00()
{
	Scene::~Scene();
}

void Level00::Awake()
{
	if (wolfResource == nullptr)
	{
		wolfResource = new WolfResource();
	}

	map = new MapInfo();
	map->mapLayer = new TileMap(
		resource.LoadTileFromTsd("./Assets/Wolf/Map/tileSet.tsd"),
		resource.LoadMapFromCSV("./Assets/Wolf/Map/���ֹ�_ͼ��� 1.csv")
	);
	map->objectLayer = new MapFile(resource.LoadMapFromCSV("./Assets/Wolf/Map/���ֹ�_�����.csv"));


	player = Player::Create(this);
	camera = new FreeCamera(&player->transform);
	mapCamera = new TileMapCamera(Vector<int>(32, 32), &player->transform);

	player->SetCamera(camera);

	for (int i = 0; i < map->objectLayer->w; i++)
	{
		for (int j = 0; j < map->objectLayer->h; j++)
		{
			if (map->objectLayer->image[j * map->objectLayer->w + i] == 18)
			{
				Caco * enimy = Caco::Create(this,new Sprite(), &player->transform, player);
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

	//����������Ʒ
	for (int i = 0; i < map->objectLayer->w; i++)
	{
		for (int j = 0; j < map->objectLayer->h; j++)
		{
			if (map->objectLayer->image[j * map->objectLayer->w + i] == 27)
			{
				Item * healthBox = Item::Create(this,&wolfResource->healthBoxSprite);
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

	//���ó�������ײ����
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
	map->mapLayer->SetCollisionTileID(collisionTileID);

	//���ݵ�ǰ������ͼ�����
	player->rigid->SetTileMap(map->mapLayer);
	//�������λ��
	player->SetPosition(35, *map->objectLayer);
	player->transform.eulerAngle = PI;
}

void Level00::Start()
{
	wolfResource->BGM->volume = 0.4;
	wolfResource->BGM->Play(true);
	inputSystem.SetRelativeMouseMode(true);
}

void Level00::Update()
{
	
	camera->Render(*map->mapLayer, false, nullptr, WHITE);
	wolfResource->mouse.DrawSprite(pixelWidth * 0.5 - 0.5 * wolfResource->mouse.scale.x, pixelHeight * 0.5 - 0.5 * wolfResource->mouse.scale.y, true);
	if (inputSystem.GetKeyHold(KeyM))
	{
		Vector<int> drawPos = mapCamera->Render(*map->mapLayer, { 0,0 }, {0,0});
		renderer.DrawRect(drawPos, drawPos + Vector<int>(32, 32), WHITE);
	}

	
}

void Level00::Unload()
{
	if (map!=nullptr)
	{
		delete map;
		Debug("ж�س����ɹ�");
	}
	delete camera;
	delete mapCamera;
	
}
