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
	
}

Level00* Level00::Create()
{
	return new Level00();
}

void Level00::Delete()
{
	delete this;
}

void Level00::Awake()
{

	allStates.clear();

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
	
	camera.Render(*map->mapLayer, false, nullptr, WHITE);
	wolfResource->mouse.DrawSprite(pixelWidth * 0.5 - 0.5 * wolfResource->mouse.scale.x, pixelHeight * 0.5 - 0.5 * wolfResource->mouse.scale.y, true);
	if (inputSystem.GetKeyHold(KEYCODE::KeyM))
	{
		Vector<int> drawPos = mapCamera.Render(*map->mapLayer, { 0,0 }, {0,0});
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
		}
	}

	allStates.clear();

}
