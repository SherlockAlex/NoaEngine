#include "GameEngine.h"
#include "Player.h"
#include "Enimy.h"
#include "Item.h"
#include "Caco.h"
using namespace noa;

/// <summary>
/// 游戏的场景管理
/// 一个便捷的开发方式是将场景作为一个类，然后游戏一开始就实例化出所有的场景
/// 这样的方式问题在于内存资源的浪费，因为玩家只能在当前的场景下进行活动，但是系统加载了游戏的所有资源
/// 
/// 这里采用了动态加载场景的方式，只要将场景保存为两属性字符串，分别表示tileMap在本地的路径
/// 然后Scene提供一个方法为GetTileMap()，返回所制造的地图便可，接着配合Game类进行一个场景的加载
/// 
/// </summary>



class Wolf3D :public NoaGameEngine
{
public:
	Wolf3D(int width, int height, NoaGameEngine::GameWindowMode windowMode, string gameName) :
		NoaGameEngine(width, height, windowMode, gameName)
	{
		

		player.SetCamera(&camera);

		//编写场景生成事件
		function<void(MapInfo*)> loadAction = [this](MapInfo* map) {
			//场景生成怪物

			for (int i = 0; i < map->objectLayer.w; i++)
			{
				for (int j = 0; j < map->objectLayer.h; j++)
				{
					if (map->objectLayer.image[j * map->objectLayer.w + i] == 18)
					{
						Caco* enimy = new Caco(new Sprite(cacoSprite), &player.transform, &player);
						enimy->SetTileMap(&map->mapLayer);
						enimy->item = &bulletSprite;
						enimy->itemPickEvent += [this]() {
							player.bulletCount += 10;
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
						Item* healthBox = new Item(&healthBoxSprite);
						healthBox->SetTileMap(&map->mapLayer);
						healthBox->transform.position.x = i;
						healthBox->transform.position.y = j;

						healthBox->pickEvent += [this]() {
							player.hp += 5;
							if (player.hp>player.maxHp) 
							{
								player.hp = player.maxHp;
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
			player.SetTileMap(&map->mapLayer);

			//设置玩家位置
			player.SetPosition(35, map->objectLayer);
		};

		sceneManager.loadAction = loadAction;

		//加载第一个场景
		sceneManager.LoadScene("NewGame");

	}

	void Start() override
	{
		BGM.volume = 0.4;
		BGM.Play(true);
		inputSystem.SetRelativeMouseMode(true);
	}

	float i = 0;
	uint32_t mutiColor = WHITE;
	uint32_t darkLight = RGB(20, 20, 20);
	void Update() override {

		if (inputSystem.GetKeyDown(KeyK))
		{
			if(mutiColor == WHITE)
			{
				mutiColor = darkLight;
			}
			else {
				mutiColor = WHITE;
			}
		}
		
		camera.Render(sceneManager.activeMapInfo.mapLayer, false, nullptr, mutiColor);
		mouse.DrawSprite(pixelWidth * 0.5 - 0.5 * mouse.scale.x, pixelHeight * 0.5 - 0.5 * mouse.scale.y, true);
		if (inputSystem.GetKeyHold(KeyM))
		{
			Vector<int> drawPos = mapCamera.Render(sceneManager.activeMapInfo.mapLayer, Vector<float>(0.0, 0.0), Vector<float>(0.0, 0.0));
			renderer.DrawRect(drawPos, drawPos + Vector<int>(32, 32), WHITE);
		}

		if (inputSystem.GetKeyHold(KeyESC))
		{
			Quit();
		}

	}

private:

	Scene newGame = Scene(
		"NewGame",
		{
			"./Assets/Wolf/Map/新手关_图块层 1.csv" ,
			"./Assets/Wolf/Map/新手关_对象层.csv",
			"./Assets/Wolf/Map/tileSet.tsd"
		}
	);

	Sprite sky = Sprite(resource.LoadSprFile("./Assets/Wolf/sky-sun.spr"), Vector<int>(1.0, 1.0));

	Player player = Player();
	FreeCamera camera = FreeCamera(&player.transform);
	TileMapCamera mapCamera = TileMapCamera(Vector<int>(32, 32), &player.transform);
	
	//资源相关
	Sprite mouse = Sprite(resource.LoadSprFile("./Assets/Wolf/mouse.spr"), Vector<int>(0.03 * pixelWidth, 0.03 * pixelWidth));

	//Sprite cacoSprite = Sprite(resource.LoadSprFile("./Assets/Wolf/caco.spr"), Vector<int>(32, 32));
	Sprite cacoSprite = Sprite(Vector<int>(32, 32));

	Sprite bulletSprite = Sprite(resource.LoadSprFile("./Assets/Wolf/bullet.spr"), Vector<int>(64, 64));
	Sprite healthBoxSprite = Sprite(resource.LoadSprFile("./Assets/Wolf/health-box.spr"), Vector<int>(64, 64));

	Audio bulletPickUpSFX = Audio("./Assets/Wolf/Music/pickUpBullet.mp3", Chunk);

	Audio BGM = Audio("./Assets/Wolf/Music/theme.mp3", Music);

};

int main(int argc,char * argv[])
{
	Wolf3D wolf3D(1920 / 2, 1080 / 2, NoaGameEngine::WindowMode, "3D");
	wolf3D.Run();
	return 0;
}