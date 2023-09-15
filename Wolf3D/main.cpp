#include "GameEngine.h"
#include "Player.h"
#include "Enimy.h"
#include "Item.h"
#include "Caco.h"
using namespace noa;

/// <summary>
/// ��Ϸ�ĳ�������
/// һ����ݵĿ�����ʽ�ǽ�������Ϊһ���࣬Ȼ����Ϸһ��ʼ��ʵ���������еĳ���
/// �����ķ�ʽ���������ڴ���Դ���˷ѣ���Ϊ���ֻ���ڵ�ǰ�ĳ����½��л������ϵͳ��������Ϸ��������Դ
/// 
/// ��������˶�̬���س����ķ�ʽ��ֻҪ����������Ϊ�������ַ������ֱ��ʾtileMap�ڱ��ص�·��
/// Ȼ��Scene�ṩһ������ΪGetTileMap()������������ĵ�ͼ��ɣ��������Game�����һ�������ļ���
/// 
/// </summary>



class Wolf3D :public NoaGameEngine
{
public:
	Wolf3D(int width, int height, NoaGameEngine::GameWindowMode windowMode, string gameName) :
		NoaGameEngine(width, height, windowMode, gameName)
	{
		

		player.SetCamera(&camera);

		//��д���������¼�
		function<void(MapInfo*)> loadAction = [this](MapInfo* map) {
			//�������ɹ���

			for (int i = 0; i < map->objectLayer.w; i++)
			{
				for (int j = 0; j < map->objectLayer.h; j++)
				{
					if (map->objectLayer.image[j * map->objectLayer.w + i] == 18)
					{
						Caco* enimy = new Caco(new Sprite(cacoSprite), &player.transform, &player);
						enimy->UpdateMap(&map->mapLayer);
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

			//����������Ʒ
			/*for (int i = 0; i < objectMap.w; i++)
			{
				for (int j = 0; j < objectMap.h; j++)
				{
					if (objectMap.image[j * objectMap.w + i] == 28)
					{
						Item* bullet = new Item(&bulletSprite);
						bullet->UpdateMap(&tileMap);
						bullet->transform.position.x = i;
						bullet->transform.position.y = j;

						bullet->pickEvent += [this]() {
							player.bulletCount += 5;
							bulletPickUpSFX.Play(false);
							};

					}
				}
			}*/

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
			map->mapLayer.SetCollisionTileID(collisionTileID);

			//���ݵ�ǰ������ͼ�����
			player.UpdateMap(&map->mapLayer);

			//�������λ��
			player.SetPosition(35, map->objectLayer);
		};

		sceneManager.loadAction = loadAction;

		//���ص�һ������
		sceneManager.LoadScene("NewGame");

	}

	void Start() override
	{
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
			"./Assets/Wolf/Map/���ֹ�_ͼ��� 1.csv" ,
			"./Assets/Wolf/Map/���ֹ�_��Ʒ��.csv",
			"./Assets/Wolf/Map/tileSet.tsd"
		}
	);

	Sprite sky = Sprite(resource.LoadSprFile("./Assets/Wolf/sky-sun.spr"), Vector<int>(1.0, 1.0));

	Player player = Player();
	FreeCamera camera = FreeCamera(&player.transform);
	TileMapCamera mapCamera = TileMapCamera(Vector<int>(32, 32), &player.transform);
	
	//��Դ���
	Sprite mouse = Sprite(resource.LoadSprFile("./Assets/Wolf/mouse.spr"), Vector<int>(0.03 * pixelWidth, 0.03 * pixelWidth));

	Sprite cacoSprite = Sprite(resource.LoadSprFile("./Assets/Wolf/caco.spr"), Vector<int>(32, 32));

	Sprite bulletSprite = Sprite(resource.LoadSprFile("./Assets/Wolf/bullet.spr"), Vector<int>(64, 64));

	Audio bulletPickUpSFX = Audio("./Assets/Wolf/Music/pickUpBullet.mp3", Chunk);

	Audio BGM = Audio("./Assets/Wolf/Music/theme.mp3", Music);

};

int main(int argc,char * argv[])
{
	Wolf3D wolf3D(1920 / 2, 1080 / 2, NoaGameEngine::WindowMode, "3D");
	wolf3D.Run();
	return 0;
}